#version 330 core

#define VOXEL_SCALE 1
#define MAX_VOXELS 1000
#define MAX_MATERIALS 10

#define RAY_BOUNCES 4

#define INF 1e+30
#define PI 3.14159

in vec2 FragPos;
out vec4 FragColor;

//camera
uniform vec3 camPos;
uniform mat4 camRot;

uniform vec2 screenScale;
uniform float fov;

//light
uniform vec3 sunDir;

//world


//structs
struct Ray {
	vec3 origin;
	vec3 direction;
};

struct HitInfo {
	bool hit;
	float hitDistance;
	vec3 hitPoint;
	vec3 surfaceNormal;
	int materialIndex;
};

//function definitions
vec3 random(vec2 pos, int frame);

HitInfo traceRay(Ray ray, int voxelCount, vec4 voxels[MAX_VOXELS]);
vec3 getRayDirection(vec2 screenPos);

bool VoxelRayIntersection(vec3 voxelPos, Ray ray, out HitInfo info);
vec3 getVoxelNormal(vec3 voxelPos, vec3 hitPoint);


void main()
{
    //voxels
	int voxelCount = 10;
	vec4 voxels[MAX_VOXELS];

	voxels[0] = vec4(0, 0, 0, 0);//cube at x:0, y:0, z:0, material:0
	voxels[1] = vec4(1, 0, 1, 0);
	voxels[2] = vec4(2, 0, 1, 0);
	voxels[3] = vec4(3, 0, 0, 1);
	voxels[4] = vec4(4, 0, 0, 1);
	voxels[5] = vec4(1, -1, 0, 1);
	voxels[6] = vec4(2, -1, 0, 2);
	voxels[7] = vec4(3, -1, -1, 2);
	voxels[8] = vec4(4, -1, -1, 2);
	voxels[9] = vec4(2, -2, -1, 2);

	vec3 materials[3];
	materials[0] = vec3(1, 0, 0);
	materials[1] = vec3(0, 1, 0);
	materials[2] = vec3(0, 0, 1);


	//camera
	Ray ray;
	ray.origin = camPos;
	ray.direction = getRayDirection(FragPos);

	FragColor = vec4(0, 0, 0, 0);
	
	float roughness = 0.05;
	float ambient = 0.4;
	
	vec3 nSunDir = normalize(sunDir);

	//raytrace
	bool hitSomething = true;
	float energyLoss;
	
	for (int i = 0; i < RAY_BOUNCES && hitSomething; i++) {
		
		HitInfo hit = traceRay(ray, voxelCount, voxels);
		hitSomething = hit.hit;
		

		//ray trace to sun
		Ray toSun;
		toSun.origin = hit.hitPoint + (hit.surfaceNormal * 0.001);
		toSun.direction = nSunDir;

		HitInfo shadingInfo = traceRay(toSun, voxelCount, voxels);


		//illumination
		energyLoss = (i + 1) * (i + 1);
		if (!shadingInfo.hit) {
			vec3 colour = materials[hit.materialIndex] * max(dot(hit.surfaceNormal, nSunDir), ambient);
			FragColor += vec4(colour / energyLoss, 1);
		}
		else {
			FragColor += vec4((materials[hit.materialIndex] * ambient) / energyLoss, 1);
		}


		//cast new ray
		if(hit.hit){
			ray.origin = hit.hitPoint + (hit.surfaceNormal * 0.001);

			vec3 roughNormal = hit.surfaceNormal + random(FragPos, 0) * roughness;
			ray.direction = reflect(ray.direction, roughNormal);
		}
	}

	//add sky colour
	FragColor += vec4(vec3(0.2, 0.8, 0.9) / (energyLoss), 1);
	FragColor.w = 1;
} 

vec3 random(vec2 pos, int frame){
	vec2 k = vec2(23.1406926327792690, 2.6651441426902251);

	float a = fract(sin(dot(pos, k)) * 43758.5453);
	float b = fract(sin(dot(pos, k)) * 52589.6536);
	float c = fract(sin(dot(pos, k)) * 45881.3912);

	return vec3(a, b, c);
}

HitInfo traceRay(Ray ray, int voxelCount, vec4 voxels[MAX_VOXELS]) {
	
	HitInfo closestHit;
	closestHit.hitDistance = INF;
	closestHit.hit = false;
	closestHit.materialIndex = -1;

	for (int i = 0; i < voxelCount; i++) {

		HitInfo hit;
		if (VoxelRayIntersection(voxels[i].xyz, ray, hit)) {

			if (hit.hitDistance < closestHit.hitDistance) {
				closestHit = hit;
				closestHit.materialIndex = int(voxels[i].w);
			}

		}

	}

	return closestHit;
}

vec3 getRayDirection(vec2 screenPos) {
	
	//adjust for fov
	vec2 correctedScreenPos = screenPos * tan((fov * PI / 180.0f) / 2);

	//correct for aspect ratio
	correctedScreenPos *= vec2(screenScale.x / screenScale.y, 1); 

	//rotate ray direction
	return (camRot * vec4(correctedScreenPos, 1, 1)).xyz;
	//return normalize((camRot * vec4(correctedScreenPos, 1, 0)).xyz);
}

bool VoxelRayIntersection(vec3 voxelPos, Ray ray, out HitInfo info) {
	
	vec3 voxelMin = (voxelPos * VOXEL_SCALE) - (VOXEL_SCALE / 2.0);
	vec3 voxelMax = (voxelPos * VOXEL_SCALE) + (VOXEL_SCALE / 2.0);

	vec3 r1 = (voxelMin - ray.origin) / ray.direction;
	vec3 r2 = (voxelMax - ray.origin) / ray.direction;

	vec3 tmin = min(r1, r2);
	vec3 tmax = max(r1, r2);

	float t1 = max(tmin.x, max(tmin.y, tmin.z));
	float t2 = min(tmax.x, min(tmax.y, tmax.z));

	info.hit = (t1 >= 0 && t1 <= t2);
	info.hitDistance = t1;
	info.hitPoint = ray.origin + ray.direction * t1;
	info.surfaceNormal = getVoxelNormal(voxelPos, info.hitPoint);
	info.materialIndex = -1;

	return info.hit;
}

vec3 getVoxelNormal(vec3 voxelPos, vec3 hitPoint) {
	
	vec3 voxelNormal = hitPoint - voxelPos;
	vec3 absVoxelNormal = abs(voxelNormal);
	
	float maxDir = max(absVoxelNormal.x, max(absVoxelNormal.y, absVoxelNormal.z));

	if (absVoxelNormal.x == maxDir) {
		if (voxelNormal.x > 0)
			return vec3(1, 0, 0);
		return vec3(-1, 0, 0);
	}

	if (absVoxelNormal.y == maxDir) {
		if (voxelNormal.y > 0)
			return vec3(0, 1, 0);
		return vec3(0, -1, 0);
	}

	if (voxelNormal.z > 0)
		return vec3(0, 0, 1);
	return vec3(0, 0, -1);
}
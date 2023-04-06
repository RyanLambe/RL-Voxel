#version 330 core

#define INF 1e+30
#define PI 3.14159

//move to uniform
#define VOXEL_SCALE 1
#define MAX_VOXELS 1000
#define MAX_MATERIALS 10

#define RAY_BOUNCES 4
#define SAMPLE_COUNT 4


//input and output
in vec2 FragPos;
out vec4 FragColor;


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


//camera
uniform vec3 camPos;
uniform mat4 camRot;

uniform vec2 screenScale;
uniform float fov;

//progressive rendering
uniform bool render;
uniform int frame;
uniform sampler2D prevFrame;
uniform float time;

//light
uniform vec3 sunDir;

//world




//function declaration
vec3 SamplePixel(Ray ray, int sampleNum, int voxelCount, vec4 voxels[MAX_VOXELS], vec3 materials[MAX_MATERIALS]);

HitInfo TraceRay(Ray ray, int voxelCount, vec4 voxels[MAX_VOXELS]);
bool HitSun(vec3 point, int voxelCount, vec4 voxels[MAX_VOXELS]);
vec3 GetRayDirection(vec2 screenPos);

bool VoxelRayIntersection(vec3 voxelPos, Ray ray, out HitInfo info);
vec3 GetVoxelNormal(vec3 voxelPos, vec3 hitPoint);

vec3 CookTorranceSpecular(vec3 toCam, vec3 normal, vec3 toLight, vec3 baseColour, float metallic, float roughness, float specular);

vec3 Random(vec2 pos, int sampleNum);


//function definitions
void main()
{
	if(!render){
		vec2 pos = (FragPos + vec2(1)) / 2;
		FragColor = texture(prevFrame, pos);
		return;
	}

	//move to uniform
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

	vec3 materials[MAX_MATERIALS];
	materials[0] = vec3(0.94, 0.2, 0.24);
	materials[1] = vec3(0.3, 0.9, 0.5);
	materials[2] = vec3(0.18, 0.52, 0.93);

	
	//pixel ray
	Ray ray;
	ray.origin = camPos;
	ray.direction = GetRayDirection(FragPos);

	vec3 finalColour = vec3(0);
	
	for(int i = 0; i < max(SAMPLE_COUNT, 1); i++){
		finalColour += SamplePixel(ray, i, voxelCount, voxels, materials);
	}

	finalColour /= max(SAMPLE_COUNT, 1);

	//add prevFrame
	vec2 pos = (FragPos + vec2(1)) / 2;
	FragColor = vec4(finalColour, 1);
	FragColor += texture(prevFrame, pos) * (frame - 1);
	FragColor /= frame;
} 

vec3 SamplePixel(Ray ray, int sampleNum, int voxelCount, vec4 voxels[MAX_VOXELS], vec3 materials[MAX_MATERIALS]){
	
	//move to uniform
	float roughness = 0.5;
	float metallic = 0.0;
	float specular = 0.5;
	float ambient = 0.1;

	vec3 rayColour = vec3(1);
	vec3 incomingLight = vec3(0);
	
	for (int i = 0; i < 2;i++){  ;//clamp(RAY_BOUNCES, 2, 16); i++) {
		
		HitInfo hit = TraceRay(ray, voxelCount, voxels);
		if(hit.hit) {

			ray.origin = hit.hitPoint + (hit.surfaceNormal * 0.001);

			vec3 randomDir = normalize(Random(FragPos, sampleNum));
			if(dot(randomDir, hit.surfaceNormal) < 0)
				randomDir = -randomDir;

			ray.direction = normalize(randomDir);

			incomingLight += vec3(0) * rayColour;
			rayColour = materials[hit.materialIndex];
		}
		else {
			incomingLight += vec3(1) * rayColour;
			break;
		}
	}

	return incomingLight;
}

HitInfo TraceRay(Ray ray, int voxelCount, vec4 voxels[MAX_VOXELS]) {
	
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

bool HitSun(vec3 point, int voxelCount, vec4 voxels[MAX_VOXELS]){

	//get ray
	Ray ray;
	ray.origin = point;
	ray.direction = sunDir;

	//check for hits
	for(int i = 0; i < voxelCount; i++) {
		HitInfo hit;
		if(VoxelRayIntersection(voxels[i].xyz, ray, hit)){
			return false;
		}
	}
	return true;
}

vec3 GetRayDirection(vec2 screenPos) {
	
	//adjust for fov
	vec2 correctedScreenPos = screenPos * tan((fov * PI / 180.0f) / 2);

	//correct for aspect ratio
	correctedScreenPos *= vec2(screenScale.x / screenScale.y, 1); 

	//rotate ray direction
	return (camRot * vec4(correctedScreenPos, 1, 1)).xyz;
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
	info.surfaceNormal = GetVoxelNormal(voxelPos, info.hitPoint);
	info.materialIndex = -1;

	return info.hit;
}

vec3 GetVoxelNormal(vec3 voxelPos, vec3 hitPoint) {
	
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


vec3 CookTorranceSpecular(vec3 toCam, vec3 normal, vec3 toLight, vec3 baseColour, float metallic, float roughness, float specular){

	//initialize variables
	vec3 halfVec = normalize(toLight + toCam);

	float nDotL = clamp(dot(normal, toLight), 0.0, 1.0); 
	float nDotC = clamp(dot(normal, toCam), 0.0, 1.0); 
	float nDotH = clamp(dot(normal, halfVec), 0.0, 1.0); 
	float cDotH = clamp(dot(toCam, halfVec), 0.0, 1.0);


	//normal distribution
	float alpha = roughness * roughness;
	float alphaSq = alpha * alpha;
	float nDotHSq = nDotH * nDotH;

	float D = nDotHSq * (alphaSq - 1.0) + 1;
	D = alphaSq / (PI * (D * D));

	
	//fresnel
	vec3 R0 = vec3(0.16 * (specular * specular));
	R0 = mix(R0, baseColour, metallic);

	vec3 F = R0 + (1.0 - R0) * pow(1.0 - cDotH, 5);
	

	//geometry attinuation
	float k = alpha / 2.0;
	float Ga = max(nDotL, 0.001) / (nDotL * (1 - k) + k);
	float Gb = max(nDotC, 0.001) / (nDotC * (1 - k) + k);

	float G = Ga * Gb;


	//return
	return (D * F * G) / (4 * max(nDotL, 0.001) * max(nDotC, 0.001));
}

vec3 Random(vec2 pos, int sampleNum){

	//random function is a modified version of gold noise by dcerisano
	//https://www.shadertoy.com/view/ltB3zD

	vec2 correctedPos = (pos + 1) / 2;
	correctedPos *= screenScale;
	
	float PHI = 1.61803398874989484820459;
	float seed = fract(time) + float(sampleNum / 10.0);

	float a = fract(tan(distance(correctedPos*PHI, correctedPos)*(seed+0.1))*correctedPos.x);
	float b = fract(tan(distance(correctedPos*PHI, correctedPos)*(seed+0.2))*correctedPos.x);
	float c = fract(tan(distance(correctedPos*PHI, correctedPos)*(seed+0.3))*correctedPos.x);

	return vec3(a, b, c);
}
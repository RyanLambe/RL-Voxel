#define VOXEL_SCALE 1
#define MAX_VOXELS 256

#define INF 1e+30
#define PI 3.14159f

cbuffer CameraBuffer : register(b0) {
	float3 position;
	matrix rotation;

	float3 screenData;//x: width, y: height, z: fov
}

struct Ray {
	float3 origin;
	float3 direction;
};

bool VoxelRayIntersection(float3 voxelPos, Ray ray, out float hitDistance) {
	
	float3 voxelMin = (voxelPos * VOXEL_SCALE) - (VOXEL_SCALE / 2.0);
	float3 voxelMax = (voxelPos * VOXEL_SCALE) + (VOXEL_SCALE / 2.0);

	float3 r1 = (voxelMin - ray.origin) / ray.direction;
	float3 r2 = (voxelMax - ray.origin) / ray.direction;

	float3 tmin = min(r1, r2);
	float3 tmax = max(r1, r2);

	float t1 = max(tmin.x, max(tmin.y, tmin.z));
	float t2 = min(tmax.x, min(tmax.y, tmax.z));

	hitDistance = t1;
	return t1 >= 0 && t1 <= t2;
}

float3 getRayDirection(float2 screenPos) {
	
	//adjust screen pos value for -1 to 1 range
	float2 correctedScreenPos = ((screenPos / screenData.xy) * 2 - float2(1, 1)); // use -1 to 1 range

	//adjust for fov
	correctedScreenPos *= tan((screenData.z * PI / 180.0f) / 2);

	//correct for aspect ratio and flip y axiz
	correctedScreenPos *= float2(screenData.x / screenData.y, -1); 

	//rotate ray direction
	return mul(rotation, float4(correctedScreenPos, 1, 0)).xyz;
}

float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
	//voxels
	int voxelCount = 10;
	float4 voxels[MAX_VOXELS];
	voxels[0] = float4(0, 0, 0, 0);//cube at x:0, y:0, z:0, material:0
	voxels[1] = float4(1, 0, 1, 0);
	voxels[2] = float4(2, 0, 0, 0);
	voxels[3] = float4(3, 0, 0, 1);
	voxels[4] = float4(4, 0, 0, 1);
	voxels[5] = float4(1, -1, 0, 1);
	voxels[6] = float4(2, -1, 0, 2);
	voxels[7] = float4(3, -1, 0, 2);
	voxels[8] = float4(4, -1, 0, 2);
	voxels[9] = float4(2, -2, 0, 2);

	float4 materials[3];
	materials[0] = float4(1, 0, 0, 1);
	materials[1] = float4(0, 1, 0, 1);
	materials[2] = float4(0, 0, 1, 1);


	//camera
	Ray ray;
	ray.origin = position;
	ray.direction = getRayDirection(pos.xy);


	//raytrace
	float minDistance = INF;
	int minIndex = -1;
	for (int i = 0; i < voxelCount; i++) {
		float distance;
		if (VoxelRayIntersection(voxels[i].xyz, ray, distance)) {

			if (distance < minDistance) {
				minDistance = distance;
				minIndex = i;
			}

		}

	}

	if (minIndex != -1) {
		return materials[(int)voxels[minIndex].w];
	}

	return float4(0, 0, 0, 1);

}
#define VOXEL_SCALE 1
#define MAX_VOXELS 256

#define INF 1e+30

cbuffer Buff {
	float4 camPos;
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
	voxels[5] = float4(1, 1, 0, 1);
	voxels[6] = float4(2, 1, 0, 2);
	voxels[7] = float4(3, 1, 0, 2);
	voxels[8] = float4(4, 1, 0, 2);
	voxels[9] = float4(2, 2, 0, 2);

	float4 materials[3];
	materials[0] = float4(1, 0, 0, 1);
	materials[1] = float4(0, 1, 0, 1);
	materials[2] = float4(0, 0, 1, 1);

	//camera
	Ray ray;
	ray.origin = camPos.xyz;
	ray.direction = float3((pos.x / 750.0f) * 2.0f - 1.0f, (pos.y / 750.0f) * 2.0f - 1.0f, 1.0f);

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
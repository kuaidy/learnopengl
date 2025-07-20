#version 460 core
layout(local_size_x = 256) in;
struct Triangle{
	vec4 v0;
	vec4 v1;
	vec4 v2;
	int objectId;
};
struct Ray{
	vec4 origin;
	vec4 direction;
};
layout(std430,binding=0) buffer Triangles{
	Triangle tris[];
};
layout(std430,binding=1) buffer RayData{
	Ray ray;
};
layout(std430,binding=2) buffer ResultData{
	int objectId;
	float minT;
}result;
layout(std430,binding = 3) buffer DebugBuffer {
    int debugData[];
};

//求射线和三角形的交点，M?ller–Trumbore 算法是一种高效的射线与三角形求交检测算法
bool rayIntersectsTriangle(vec3 orig,vec3 dir,vec3 v0,vec3 v1,vec3 v2,out float t,uint idx)
{
	uint baseIndex = idx * 4;

	float eps = 1e-5;
	vec3 edge1=v1-v0;
	vec3 edge2=v2-v0;
	vec3 h=cross(dir,edge2);
	float a=dot(edge1,h);
	if(abs(a)<eps) return false;

	float f=1.0/a;
	vec3 s=orig-v0;
	float u=f*dot(s,h);
	if(u<0.0||u>1.0) return false;

	vec3 q=cross(s,edge1);
	float v=f*dot(dir,q);
	if(v<0.0||(u+v)>1.0) return false;

	float tempT=f*dot(edge2,q);
	if(tempT>eps){
		t=tempT;
		return true;
	}
	return false;
};

shared uint sharedT;
shared uint sharedId;

void main() {
    uint idx = gl_GlobalInvocationID.x;
    uint baseIndex = idx * 4;
    debugData[baseIndex + 0] = int(idx);
    debugData[baseIndex + 1] = int(tris.length());


	if (idx >= tris.length()) {
        return;
    }

	if (gl_LocalInvocationIndex == 0) {
		sharedT = 1000000; // 即 UINT_MAX
		sharedId=1000000;
		debugData[baseIndex + 2] = int(sharedT);
	}

	Triangle tri = tris[idx];
	float t;
	if(rayIntersectsTriangle(ray.origin.xyz,ray.direction.xyz,tri.v0.xyz,tri.v1.xyz,tri.v2.xyz,t,idx))
	{	
		
		//每个线程写入t，和模型的id
		//sharedT[gl_LocalInvocationIndex] = t;
		//sharedId[gl_LocalInvocationIndex] = tri.objectId;
		
		uint packedT = uint(t * 10000.0);
        // Atomically find minimum distance
        uint oldT = atomicMin(sharedT, packedT);
        // If this thread found a closer intersection, update the object ID
        if (packedT < oldT) {
            sharedId = tri.objectId;
        }
		memoryBarrierShared();
		result.objectId = int(sharedId);
		debugData[baseIndex + 3] = int(sharedId);
	}
}
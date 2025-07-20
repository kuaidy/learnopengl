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
	float tMin;
};
layout(std430,binding = 3) buffer DebugBuffer {
    int debugData[];
};

shared float sharedT;
shared int sharedId;

//求射线和三角形的交点，M?ller–Trumbore 算法是一种高效的射线与三角形求交检测算法
bool rayIntersectsTriangle(vec3 orig,vec3 dir,vec3 v0,vec3 v1,vec3 v2,out float t,uint idx)
{
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

//原子操作，float之间获取最小值
shared uint minBits;

void atomicMinFloat(float value){
	uint v =floatBitsToUint(value);
	while(true){
		uint expect=minBits;
		uint desired=min(v,expect);
		if(desired==expect){
			break;
		}
		if(atomicCompSwap(minBits, expect, desired) == expect){
			break;
		}
	}
}

void main(){
	uint idx=gl_GlobalInvocationID.x;

	uint baseIndex = idx * 4;

	debugData[baseIndex + 0] = int(idx);
    debugData[baseIndex + 1] = int(tris.length());

	if(idx>=tris.length()) return ;
	
	Triangle tri = tris[idx];
    float t;
	float tmpSharedT=sharedT;
	float tmpMin=tMin;

	if(rayIntersectsTriangle(ray.origin.xyz, ray.direction.xyz, tri.v0.xyz, tri.v1.xyz, tri.v2.xyz,t,idx)){
	
		uint leader=0;
		if(gl_LocalInvocationIndex ==0){
			tmpSharedT = 1e9;
            sharedId = -1;
		}
		barrier();

		minBits =floatBitsToUint(tmpSharedT);
		atomicMinFloat(t);
		tmpSharedT=uintBitsToFloat(minBits);

        memoryBarrierShared();
        barrier();

        if (abs(tmpSharedT - t) < 1e-4) {
            sharedId = tri.objectId;
        }
        barrier();

        if (gl_LocalInvocationIndex == 0) {
			minBits=floatBitsToUint(tmpMin);
			atomicMinFloat(t);
			tmpMin=uintBitsToFloat(minBits);

            if (abs(tmpSharedT - tmpMin) < 1e-4) {
                objectId = sharedId;
            }
        }
	}
};
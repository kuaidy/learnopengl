#include "ObjModel.h"
bool ObjModel::LoadFile(string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene) {
		std::cout<<"load model failed"<<std::endl;
		return false;
	}
}
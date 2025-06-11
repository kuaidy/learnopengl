#pragma once
#include <string>
#include <QOpenGLFunctions_4_5_Core>
#include <Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <stb_image.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Model
{
public:
	Model(QOpenGLFunctions_4_5_Core* qOpengGlFunction);
	/// <summary>
	/// ����ģ��
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool LoadFile(string path);
	/// <summary>
	/// ��ȡ�ڵ�
	/// </summary>
	/// <param name="node"></param>
	/// <param name="scene"></param>
	void ProcessNode(aiNode* node, const aiScene* scene);
	/// <summary>
	/// ��ȡmesh
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="scene"></param>
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="mat"></param>
	/// <param name="type"></param>
	/// <param name="typeName"></param>
	/// <returns></returns>
	vector<Texture> GetMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	/// <summary>
	/// ���ļ��м�������
	/// </summary>
	/// <param name="path"></param>
	/// <param name="directory"></param>
	/// <param name="gamma"></param>
	/// <returns></returns>
	unsigned int TextureFromFile(const char* path, const string& directory);
	/// <summary>
	/// ����ģ��
	/// </summary>
	void Draw(QOpenGLShaderProgram &shader);

private:
	QOpenGLFunctions_4_5_Core* m_QOpengGlFunction;
	vector<Mesh> m_Meshes;
	string m_TextureDir;
	vector<Texture> m_LoadedTextures;
};


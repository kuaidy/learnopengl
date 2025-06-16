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
#include <QVector3D>

using namespace std;
using namespace std::chrono;


class BindingBox {
public:
	QVector3D Min;
	QVector3D Max;
};

class Model
{
public:
	Model(QOpenGLFunctions_4_5_Core* qOpengGlFunction);
	/// <summary>
	/// 加载模型
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool LoadFile(string path);
	/// <summary>
	/// 获取节点
	/// </summary>
	/// <param name="node"></param>
	/// <param name="scene"></param>
	void ProcessNode(aiNode* node, const aiScene* scene);
	/// <summary>
	/// 获取mesh
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="scene"></param>
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	/// <summary>
	/// 加载纹理
	/// </summary>
	/// <param name="mat"></param>
	/// <param name="type"></param>
	/// <param name="typeName"></param>
	/// <returns></returns>
	vector<Texture> GetMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	/// <summary>
	/// 从文件中加载纹理
	/// </summary>
	/// <param name="path"></param>
	/// <param name="directory"></param>
	/// <param name="gamma"></param>
	/// <returns></returns>
	unsigned int TextureFromFile(const char* path, const string& directory);
	/// <summary>
	/// 绘制模型
	/// </summary>
	void Draw(QOpenGLShaderProgram &shader);
	/// <summary>
	/// 显示包围盒
	/// </summary>
	void ShowBindingBox(int viewWidth,int viewHeight, QOpenGLShaderProgram &shader, QMatrix4x4 model, QMatrix4x4 view, QMatrix4x4 projection);

	BindingBox BindingBox;
	bool IsSelected = false;
	bool IsShow = true;
private:
	QOpenGLFunctions_4_5_Core* m_QOpengGlFunction;
	vector<Mesh> m_Meshes;
	string m_TextureDir;
	vector<Texture> m_LoadedTextures;
};
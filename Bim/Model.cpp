#include "Model.h"
#include <Logger.h>

Model::Model(QOpenGLFunctions_4_5_Core* qOpengGlFunction) {
	this->m_QOpengGlFunction = qOpengGlFunction;
}

/// <summary>
/// 加载模型
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
bool Model::LoadFile(string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene) {
		std::cout << "load model failed" << std::endl;
		return false;
	}
	m_TextureDir = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

/// <summary>
/// 获取节点
/// </summary>
/// <param name="node"></param>
/// <param name="scene"></param>
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	auto start = high_resolution_clock::now();
	for (int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
	auto end = high_resolution_clock::now();
	string message = "耗时: " + std::to_string(duration_cast<milliseconds>(end - start).count()) + "ms";
	Logger logger;
	logger.WriteInfo(message);
}
/// <summary>
/// 获取mesh
/// </summary>
/// <param name="mesh"></param>
/// <param name="scene"></param>
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	Vertex vertex;
	//顶点数据
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		QVector3D position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Position = position;
		//法向量
		if (mesh->HasNormals()) {
			QVector3D normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			vertex.Normal = normal;
		}
		//获取纹理坐标
		if (mesh->mTextureCoords[0]) {
			QVector2D texCoords(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vertex.TexCoords = texCoords;
		}
		vertices.push_back(vertex);
	}

	vector<unsigned int> indices;
	//获取顶点索引
	for (int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//获取纹理贴图
	vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//环境光贴图
	vector<Texture> diffuseMaps = GetMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = GetMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = GetMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = GetMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(m_QOpengGlFunction, vertices, indices, textures);
}

vector<Texture> Model::GetMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
	vector<Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
		{
			if (std::strcmp(m_LoadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_LoadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->m_TextureDir);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char* path, const string& directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	m_QOpengGlFunction->glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		m_QOpengGlFunction->glBindTexture(GL_TEXTURE_2D, textureID);
		m_QOpengGlFunction->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		m_QOpengGlFunction->glGenerateMipmap(GL_TEXTURE_2D);

		m_QOpengGlFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		m_QOpengGlFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		m_QOpengGlFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		m_QOpengGlFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
/// <summary>
/// 绘制模型
/// </summary>
void Model::Draw(QOpenGLShaderProgram& shader)
{
	for (int i = 0; i < m_Meshes.size(); ++i) {
		m_Meshes[i].Draw(shader);
	}
}

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

	/*auto end = high_resolution_clock::now();
	string message = "耗时: " + std::to_string(duration_cast<milliseconds>(end - start).count()) + "ms";
	Logger logger;
	logger.WriteInfo(message);*/
}
/// <summary>
/// 获取mesh
/// </summary>
/// <param name="mesh"></param>
/// <param name="scene"></param>
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{

	QVector3D minPt(std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max(),
		std::numeric_limits<float>::max());

	QVector3D maxPt(-std::numeric_limits<float>::max(),
		-std::numeric_limits<float>::max(),
		-std::numeric_limits<float>::max());
	// data to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			//// tangent
			//vector.x = mesh->mTangents[i].x;
			//vector.y = mesh->mTangents[i].y;
			//vector.z = mesh->mTangents[i].z;
			//vertex.Tangent = vector;
			//// bitangent
			//vector.x = mesh->mBitangents[i].x;
			//vector.y = mesh->mBitangents[i].y;
			//vector.z = mesh->mBitangents[i].z;
			//vertex.Bitangent = vector;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);

		//求包围盒
		minPt.setX(std::min(minPt.x(), mesh->mVertices[i].x));
		minPt.setY(std::min(minPt.y(), mesh->mVertices[i].y));
		minPt.setZ(std::min(minPt.z(), mesh->mVertices[i].z));

		maxPt.setX(std::max(maxPt.x(), mesh->mVertices[i].x));
		maxPt.setY(std::max(maxPt.y(), mesh->mVertices[i].y));
		maxPt.setZ(std::max(maxPt.z(), mesh->mVertices[i].z));
		this->m_BindingBox.Min = minPt;
		this->m_BindingBox.Max = maxPt;
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	vector<Texture> diffuseMaps = GetMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = GetMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = GetMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//// 4. height maps
	//std::vector<Texture> heightMaps = GetMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(m_QOpengGlFunction, vertices, indices, textures);



	//vector<Vertex> vertices;
	//Vertex vertex;

	////顶点数据
	//for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
	//	glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
	//	vertex.Position = position;
	//	//法向量
	//	if (mesh->HasNormals()) {
	//		glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
	//		vertex.Normal = normal;
	//	}
	//	//获取纹理坐标
	//	if (mesh->mTextureCoords[0]) {
	//		glm::vec2 texCoords(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	//		vertex.TexCoords = texCoords;
	//	}
	//	vertices.push_back(vertex);

	//	//求包围盒
	//	minPt.setX(std::min(minPt.x(), mesh->mVertices[i].x));
	//	minPt.setY(std::min(minPt.y(), mesh->mVertices[i].y));
	//	minPt.setZ(std::min(minPt.z(), mesh->mVertices[i].z));

	//	maxPt.setX(std::max(maxPt.x(), mesh->mVertices[i].x));
	//	maxPt.setY(std::max(maxPt.y(), mesh->mVertices[i].y));
	//	maxPt.setZ(std::max(maxPt.z(), mesh->mVertices[i].z));
	//	this->m_BindingBox.Min = minPt;
	//	this->m_BindingBox.Max = maxPt;
	//}

	//vector<unsigned int> indices;
	////获取顶点索引
	//for (int i = 0; i < mesh->mNumFaces; ++i) {
	//	aiFace face = mesh->mFaces[i];
	//	for (int j = 0; j < face.mNumIndices; ++j) {
	//		indices.push_back(face.mIndices[j]);
	//	}
	//}
	////获取纹理贴图
	//vector<Texture> textures;
	//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	////环境光贴图
	//vector<Texture> diffuseMaps = GetMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//// 2. specular maps
	//vector<Texture> specularMaps = GetMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//// 3. normal maps
	//std::vector<Texture> normalMaps = GetMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//// 4. height maps
	//std::vector<Texture> heightMaps = GetMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	//return Mesh(m_QOpengGlFunction, vertices, indices, textures);
}

vector<Texture> Model::GetMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
		{
			if (std::strcmp(m_LoadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_LoadedTextures[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->m_TextureDir);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_LoadedTextures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
		}
	}
	return textures;

	/*vector<Texture> textures;
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
	return textures;*/
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
/// <summary>
/// 显示包围盒
/// </summary>
void Model::ShowBindingBox(int viewWidth, int viewHeight, QOpenGLShaderProgram &shader, QMatrix4x4 model, QMatrix4x4 view, QMatrix4x4 projection) {
	vector<QVector3D> verteies =
	{
		{m_BindingBox.Min.x(), m_BindingBox.Min.y(), m_BindingBox.Min.z()},
		{m_BindingBox.Min.x(), m_BindingBox.Max.y(), m_BindingBox.Min.z()},
		{m_BindingBox.Min.x(), m_BindingBox.Max.y(), m_BindingBox.Max.z()},
		{m_BindingBox.Min.x(), m_BindingBox.Min.y(), m_BindingBox.Max.z()},
		{m_BindingBox.Max.x(), m_BindingBox.Min.y(), m_BindingBox.Min.z()},
		{m_BindingBox.Max.x(), m_BindingBox.Max.y(), m_BindingBox.Min.z()},
		{m_BindingBox.Max.x(), m_BindingBox.Max.y(), m_BindingBox.Max.z()},
		{m_BindingBox.Max.x(), m_BindingBox.Min.y(), m_BindingBox.Max.z()}
	};
	unsigned int indices[] = {
		// Front face (z = min.z): 0, 1, 5, 4
		0, 1, 5,
		0, 5, 4,

		// Back face (z = max.z): 3, 7, 6, 2
		3, 7, 6,
		3, 6, 2,

		// Left face (x = min.x): 0, 3, 2, 1
		0, 3, 2,
		0, 2, 1,

		// Right face (x = max.x): 4, 5, 6, 7
		4, 5, 6,
		4, 6, 7,

		// Top face (y = max.y): 1, 2, 6, 5
		1, 2, 6,
		1, 6, 5,

		// Bottom face (y = min.y): 0, 4, 7, 3
		0, 4, 7,
		0, 7, 3
	};

	unsigned int vao, vbo, ebo;
	m_QOpengGlFunction->glGenVertexArrays(1, &vao);
	m_QOpengGlFunction->glBindVertexArray(vao);

	m_QOpengGlFunction->glGenBuffers(1, &vbo);
	m_QOpengGlFunction->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_QOpengGlFunction->glBufferData(GL_ARRAY_BUFFER, verteies.size()*sizeof(QVector3D), verteies.data(), GL_STATIC_DRAW);
	
	std::cout << m_QOpengGlFunction->glGetError() << std::endl;

	m_QOpengGlFunction->glGenBuffers(1, &ebo);
	m_QOpengGlFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	m_QOpengGlFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_QOpengGlFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
	m_QOpengGlFunction->glEnableVertexAttribArray(0);

	//// 假设每个顶点 6 个 float（3 position + 3 color）
	//m_QOpengGlFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 位置
	//m_QOpengGlFunction->glEnableVertexAttribArray(0);

	//m_QOpengGlFunction->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // 颜色
	//m_QOpengGlFunction->glEnableVertexAttribArray(1);

	shader.bind();

	//模型矩阵
	shader.setUniformValue("model", model);

	//观察矩阵
	shader.setUniformValue("view", view);

	//投影矩阵
	shader.setUniformValue("projection", projection);

	m_QOpengGlFunction->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	shader.release();
}

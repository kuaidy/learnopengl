#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>


#define MAX_BONE_INFLUENCE 4

using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	
	// position
	//QVector3D Position;
	//// normal
	//QVector3D Normal;
	//// texCoords
	//QVector2D TexCoords;
	//// tangent
	//glm::vec3 Tangent;
	//// bitangent
	//glm::vec3 Bitangent;
	////bone indexes which will influence this vertex
	//int m_BoneIDs[MAX_BONE_INFLUENCE];
	////weights from each bone
	//float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh
{
public:
	Mesh(QOpenGLFunctions_4_5_Core* qOpengGlFunction, vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture>      textures;
	void Draw(QOpenGLShaderProgram& shaderProgram);
	void ShowBindBox();
private:
	unsigned int VAO;
	unsigned int VBO, EBO;
	QOpenGLFunctions_4_5_Core* m_QOpengGlFunction;
	void SetupMesh();
};


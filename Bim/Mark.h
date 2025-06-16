#pragma once
#include <vector>
#include <QPoint>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_4_5_Core>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H  

struct Character {
	GLuint TextreID;//字形纹理id
	unsigned int Size[2];//字形大小
	unsigned int Bearing[2];//从基准线到字形左部/顶部的偏移值
	GLuint Advance;//距离下一个字形原点的距离
};

class Mark {
public:
	Mark(QOpenGLFunctions_4_5_Core* qOpenGLFunction);
	~Mark();
	void DrawLine(
		int viewWidth,
		int viewHeight,
		QMatrix4x4 model,
		QMatrix4x4 view,
		QMatrix4x4 projection);
	std::vector<QVector3D> LinePoints;
	std::map<GLchar, Character> Characters;
	void LoadCharacters();
	void DrawTxt(std::string text, GLfloat x, GLfloat y, GLfloat scale, QVector3D color,int w,int h);
private:
	QOpenGLFunctions_4_5_Core* m_QOpenGLFunction;
	QOpenGLShaderProgram* m_TxtShader;
	QOpenGLShaderProgram* m_MarkShader;
};
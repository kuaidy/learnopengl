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
	GLuint TextreID;//��������id
	unsigned int Size[2];//���δ�С
	unsigned int Bearing[2];//�ӻ�׼�ߵ�������/������ƫ��ֵ
	GLuint Advance;//������һ������ԭ��ľ���
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
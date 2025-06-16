#include "Mark.h"


Mark::Mark(QOpenGLFunctions_4_5_Core* qOpengGlFunction) {
	m_QOpenGLFunction = qOpengGlFunction;

	m_MarkShader = new QOpenGLShaderProgram();
	if (!m_MarkShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/MarkVertexShader.glsl")) {
		qDebug() << "Vertex shader error:" << m_MarkShader->log();
	}
	if (!m_MarkShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/MarkFragmentShader.glsl")) {
		qDebug() << "Fragment shader error:" << m_MarkShader->log();
	}
	if (!m_MarkShader->link()) {
		qDebug() << "Shader program link error:" << m_MarkShader->log();
	}

	m_TxtShader = new QOpenGLShaderProgram();
	if (!m_TxtShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./Shaders/TextVertexShader.glsl")) {
		qDebug() << "Vertex shader error:" << m_TxtShader->log();
	}
	if (!m_TxtShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./Shaders/TextFragmentShader.glsl")) {
		qDebug() << "Fragment shader error:" << m_TxtShader->log();
	}
	if (!m_TxtShader->link()) {
		qDebug() << "Shader program link error:" << m_TxtShader->log();
	}
	LoadCharacters();
}

Mark::~Mark() {
	delete m_TxtShader;
	delete m_MarkShader;
}

void Mark::DrawLine(int viewWidth,
	int viewHeight,
	QMatrix4x4 model,
	QMatrix4x4 view,
	QMatrix4x4 projection
	) {
	if (LinePoints.size() < 2) return;
	unsigned int vao, vbo, ebo;
	m_QOpenGLFunction->glGenVertexArrays(1, &vao);
	m_QOpenGLFunction->glBindVertexArray(vao);

	m_QOpenGLFunction->glGenBuffers(1, &vbo);
	m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_QOpenGLFunction->glBufferData(GL_ARRAY_BUFFER, LinePoints.size() * sizeof(QVector3D), LinePoints.data(), GL_STATIC_DRAW);

	//m_QOpenGLFunction->glGenBuffers(1, &ebo);
	//m_QOpenGLFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//m_QOpenGLFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_QOpenGLFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);
	m_QOpenGLFunction->glEnableVertexAttribArray(0);

	m_MarkShader->bind();
	//模型矩阵
	//m_Shader->setUniformValue("model", model);
	//观察矩阵
	m_MarkShader->setUniformValue("view", view);
	//投影矩阵
	m_MarkShader->setUniformValue("projection", projection);
	//m_QOpenGLFunction->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	m_QOpenGLFunction->glDrawArrays(GL_LINES, 0, 2);
	m_MarkShader->release();

}


void Mark::LoadCharacters() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "init freetype failed"<<std::endl;
	}
	FT_Face face;
	if (FT_New_Face(ft,"./Fonts/simsun.ttc",0,&face)) {
		std::cout << "new face failed" << std::endl;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	m_QOpenGLFunction->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//禁用字节对齐限制
	for (GLubyte i = 0; i < 128;i++) {
		//加载字符的字形
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			std::cout << "char load failed" << std::endl;
			continue;
		}
		//生成纹理
		unsigned int texture;
		m_QOpenGLFunction->glGenTextures(1,&texture);
		m_QOpenGLFunction->glBindTexture(GL_TEXTURE_2D, texture);
		m_QOpenGLFunction->glTexImage2D(GL_TEXTURE_2D,0,GL_RED,face->glyph->bitmap.width,face->glyph->bitmap.rows,0,GL_RED,GL_UNSIGNED_BYTE,face->glyph->bitmap.buffer);
		//设置纹理选项
		m_QOpenGLFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_QOpenGLFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_QOpenGLFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_QOpenGLFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//存储字符
		Character character = {
			texture,
			{face->glyph->bitmap.width,face->glyph->bitmap.rows},
			{face->glyph->bitmap_left,face->glyph->bitmap_top},
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(i, character));
	}
	m_QOpenGLFunction->glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Mark::DrawTxt(std::string text, GLfloat x, GLfloat y, GLfloat scale, QVector3D color,int w,int h) {

	unsigned int vao, vbo;
	m_QOpenGLFunction->glGenVertexArrays(1,&vao);
	m_QOpenGLFunction->glBindVertexArray(vao);
	m_QOpenGLFunction->glGenBuffers(1,&vbo);
	m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	m_QOpenGLFunction->glBufferData(GL_ARRAY_BUFFER,6*4*sizeof(GLfloat),NULL,GL_DYNAMIC_DRAW);
	m_QOpenGLFunction->glEnableVertexAttribArray(0);
	m_QOpenGLFunction->glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),(void*)0);
	m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER,0);
	m_QOpenGLFunction->glBindVertexArray(0);

	m_TxtShader->bind();

	QMatrix4x4 projection;
	projection.ortho(0.0f, w, 0.0f, h, -0.1f, 100.0f);
	m_TxtShader->setUniformValue("projection", projection);

	m_TxtShader->setUniformValue("textColor",color.x(),color.y(), color.z());
	m_QOpenGLFunction->glActiveTexture(GL_TEXTURE0);
	m_QOpenGLFunction->glBindVertexArray(vao);
	//遍历文本中所有的字符
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[*c];
		GLfloat xpos = x + ch.Bearing[0] * scale;
		GLfloat ypos = y + ch.Bearing[1] * scale;
		GLfloat w = ch.Size[0] * scale;
		GLfloat h = ch.Size[1] * scale;
		//对每个字符更新vbo
		GLfloat vertices[6][4] = {
			{xpos,ypos + h,0.0,0.0},
			{xpos,ypos,0.0,1.0},
			{xpos + w,ypos,1.0,1.0},
			{xpos,ypos + h,0.0,0.0},
			{xpos + w,ypos,1.0,1.0},
			{xpos + w,ypos + h,1.0,0.0}
		};
		//在四边形上绘制字形纹理
		m_QOpenGLFunction->glBindTexture(GL_TEXTURE_2D,ch.TextreID);
		m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER,vbo);
		m_QOpenGLFunction->glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices), vertices);
		m_QOpenGLFunction->glBindBuffer(GL_ARRAY_BUFFER,0);
		m_QOpenGLFunction->glDrawArrays(GL_TRIANGLES,0,6);
		x += (ch.Advance >> 6) * scale;
	}
	m_QOpenGLFunction->glBindVertexArray(0);
	m_QOpenGLFunction->glBindTexture(GL_TEXTURE_2D,0);
	m_TxtShader->release();
}

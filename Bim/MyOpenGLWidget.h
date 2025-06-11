#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <assimp/scene.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <string>
#include <QOpenGLVersionFunctionsFactory>
#include "Model.h"
#include "Mesh.h"

class MyOpenGLWidget:public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
public:
    explicit MyOpenGLWidget(QWidget* parent = nullptr);
    ~MyOpenGLWidget();
protected:
     void initializeGL() override;
     void resizeGL(int w, int h) override;
     void paintGL() override;
private:
    Model* m_Model;
    
    QOpenGLShaderProgram* m_program;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    GLuint m_vao;
    
};


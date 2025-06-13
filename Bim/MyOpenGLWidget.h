#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <assimp/scene.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <string>
#include <QOpenGLVersionFunctionsFactory>
#include <QMouseEvent>
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
     void mousePressEvent(QMouseEvent* event) override;
private:
    Model* m_Model;
    QOpenGLShaderProgram* m_Shader;
    QOpenGLShaderProgram* m_CubeShader;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    GLuint m_vao;
    vector<QVector3D> m_Points;

    QVector3D ScreenToWorld(int x,int y);
};


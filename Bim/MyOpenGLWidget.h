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
#include "Mark.h"

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
    /// <summary>
    /// 模型矩阵
    /// </summary>
    QMatrix4x4 m_MatrixModel;
    /// <summary>
    /// 观察矩阵
    /// </summary>
    QMatrix4x4 m_MatrixView;
    /// <summary>
    /// 投影矩阵
    /// </summary>
    QMatrix4x4 m_MatrixProjection;

    QVector3D ScreenToWorld(int x,int y);
    /// <summary>
    /// 判断鼠标点击是否与模型的包围盒相交
    /// </summary>
    /// <returns></returns>
    bool RayIntersectsAABB(const QVector3D& rayOrigin,
        const QVector3D& rayDir,
        const QVector3D& boxMin,
        const QVector3D& boxMax,
        float& tmin,
        float& tmax);
    QVector3D ScreenPosToRayDir(int x,int y);

    Mark* m_Mark;
};


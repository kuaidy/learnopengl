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
#include "ParametricModeling.h"
#include "CommandMode.h"
#include <ModelLine.h>

class MyOpenGLWidget:public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
public:
    explicit MyOpenGLWidget(QWidget* parent = nullptr);
    ~MyOpenGLWidget();
    CommandMode CommandMode = CommandMode::None;
    void CreateModelLine();
protected:
     void initializeGL() override;
     void resizeGL(int w, int h) override;
     void paintGL() override;
     void mousePressEvent(QMouseEvent* event) override;
     void mouseReleaseEvent(QMouseEvent* event) override;
     void mouseMoveEvent(QMouseEvent* event) override;
     void wheelEvent(QWheelEvent* event) override;
     void keyPressEvent(QKeyEvent* event) override;
     void keyReleaseEvent(QKeyEvent* event) override;
private:

	QOpenGLFunctions_4_5_Core* m_QOpengGlFunction;

	std::shared_ptr<ModelLine> m_ModelLine;

    Model* m_Model;
    QOpenGLShaderProgram* m_Shader;
    QOpenGLShaderProgram* m_CubeShader;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    GLuint m_vao;

    QVector3D m_CameraPos;
    QVector3D m_CameraTarget;
    QVector3D m_Up;

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
    /// <summary>
    /// 标注
    /// </summary>
    Mark* m_Mark;
    /// <summary>
    /// 参数化建模
    /// </summary>
    ParametricModeling* m_ParametricModeling;
    /// <summary>
    /// 缩放
    /// </summary>
    void Scale();
    bool m_IsAltPress=false;
    bool m_IsRightMousePress=false;
    QPoint m_RightMousePoint;
    float m_Distance = 3.0f;
    float m_Yaw = 0.0f;
    float m_Pitch = 0.0f;
   
};


#include <vector>
#include <QPoint>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_4_5_Core>

class Mark {
public:
	Mark();
	void DrawLine(QPoint x, QPoint y);

private:
	QOpenGLFunctions_4_5_Core* m_QOpenGLFunctions;
};
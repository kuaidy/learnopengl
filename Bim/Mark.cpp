#include "Mark.h"


Mark::Mark() {
	m_QOpenGLFunctions= QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(QOpenGLContext::currentContext());
}
unsigned int vao;
void Mark::DrawLine(QPoint x, QPoint y) {
	
}
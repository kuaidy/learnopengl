#include "RenderContext.h"

RenderContext& RenderContext::Instance() {
	static RenderContext instance;
	return instance;
}

void RenderContext::Initialize(QOpenGLFunctions_4_5_Core* glFuncs,
	int width,
	int height,
	QMatrix4x4 matrixModel,
	QMatrix4x4 matrixView,
	QMatrix4x4 matrixProjection) {
	this->gl = glFuncs;
	this->width = width;
	this->height = height;
	this->matrix_model = matrixModel;
	this->matrix_view = matrixView;
	this->matrix_projection = matrixProjection;
}
#include "Transform.h"
QVector3D Transform::ScreenToWorld(int x, int y,int width,int height,QMatrix4x4 projection, QMatrix4x4 view) {
	float ndcX = (2.0f * x) / width - 1.0f;
	float ndcY = 1.0f - (2.0f * y) / height;

	QVector4D nearPoint(ndcX, ndcY, -1.0f, 1.0f);
	QVector4D farPoint(ndcX, ndcY, 1.0f, 1.0f);

	//ƒÊæÿ’Û
	QMatrix4x4 inv = (projection * view).inverted();

	QVector4D worldNear = inv * nearPoint;
	QVector4D worldFar = inv * farPoint;

	worldNear /= worldNear.w();
	worldFar /= worldFar.w();

	//ππ‘Ï…‰œﬂ
	QVector3D rayOrigin = worldNear.toVector3D();
	QVector3D rayDir = (worldFar - worldNear).toVector3D().normalized();

	float t = -rayOrigin.z() / rayDir.z();
	QVector3D pointOnPlane = rayOrigin + t * rayDir;
	return pointOnPlane;
}
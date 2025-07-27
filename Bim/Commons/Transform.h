#pragma once
#include <QVector3D>
#include <QMatrix4x4>
class Transform
{
public:
	static QVector3D ScreenToWorld(int x, int y, int width, int height, QMatrix4x4 projection, QMatrix4x4 view);
};


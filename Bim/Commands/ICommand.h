#pragma once
#include <QMouseEvent>
class ICommand
{
public:
	virtual void Execute() = 0;
	virtual void Finish() = 0;
	virtual void Cancel() = 0;
	virtual void OnMousePress(QMouseEvent* event) = 0;
	virtual void OnMouseMove(QMouseEvent* event) = 0;
	virtual void OnMouseRelease(QMouseEvent* event) = 0;
	virtual void OnDraw() = 0;
};


#pragma once
#include <QMouseEvent>
#include "../Document.h"

class ICommand
{
public:
	virtual void Execute() = 0;
	virtual void Finish(std::shared_ptr<Bim::Document> document) = 0;
	virtual void Cancel() = 0;
	virtual void OnMousePress(QMouseEvent* event) = 0;
	virtual void OnMouseMove(QMouseEvent* event) = 0;
	virtual void OnMouseRelease(QMouseEvent* event) = 0;
	virtual void OnDraw() = 0;
	virtual ~ICommand() = default;
};


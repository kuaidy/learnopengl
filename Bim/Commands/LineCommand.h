#pragma once
#include "ICommand.h"
#include "Line.h"
class LineCommand:public ICommand
{
public:
	LineCommand();
	void Execute() override;
	void Finish() override;
	void Cancel() override;
	void OnMousePress(QMouseEvent* event) override;
	void OnMouseMove(QMouseEvent* event) override;
	void OnMouseRelease(QMouseEvent* event) override;
	void OnDraw() override;
private:
	std::shared_ptr<Line> m_Line;
};


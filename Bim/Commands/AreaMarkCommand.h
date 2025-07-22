#pragma once
#include "ICommand.h"
class AreaMarkCommand :public ICommand
{
public:
	void Execute();
	void Undo();
	void Finish();
};


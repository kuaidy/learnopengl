#pragma once

#include <QWidget>
#include "ui_BuildView.h"

class BuildView : public QWidget
{
	Q_OBJECT

public:
	BuildView(QWidget *parent = nullptr);
	~BuildView();
	
private:
	Ui::BuildViewClass ui;
};


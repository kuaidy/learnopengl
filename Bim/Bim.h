#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Bim.h"
#include <QPushButton>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>
#include "MyOpenGLWidget.h"

using namespace std;

class Bim : public QMainWindow
{
	Q_OBJECT

public:
	Bim(QWidget* parent = nullptr);
	~Bim();
	void OpenModule(string path);
private slots:
	void on_btnopenfile_clicked();
private:
	Ui::BimClass ui;
	MyOpenGLWidget* myOpenGlWidget;
};


#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BimMainWindow.h"
#include <QPushButton>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>
#include <QFileDialog>
#include "MyOpenGLWidget.h"
#include "FileLoadFactory.h"

using namespace std;

class BimMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	BimMainWindow(QWidget* parent = nullptr);
	~BimMainWindow();
	void OpenModule(string path);

public slots:
	void on_fileopen_triggered();
	void on_modelline_triggered(bool flag);
	void on_bspline_triggered(bool flag);
	void on_ball_triggered(bool flag);
private:
	Ui::BimClass ui;
	MyOpenGLWidget* m_MyOpenGlWidget;
};


#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BimMainWindow.h"
#include <QPushButton>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QtVariantPropertyManager>

#include "MyOpenGLWidget.h"
#include "FileLoadFactory.h"
#include <ui/EditView.h>
#include <ui/BuildView.h>
#include "Document.h"

using namespace std;

class BimMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	BimMainWindow(
		QWidget* parent = nullptr, 
		std::shared_ptr<CommandManager> commandManager = nullptr, 
		std::shared_ptr<Document> document=nullptr);
	~BimMainWindow();
	void OpenModule(string path);
	std::shared_ptr<FileLoader> file_loader;
	
public slots:
	void on_fileopen_triggered();
	void on_modelline_triggered(bool flag);
	void on_bspline_triggered(bool flag);
	void on_ball_triggered(bool flag);
	void on_menu_edit_triggered();
	void on_build_triggered();
private:
	Ui::BimClass ui;
	MyOpenGLWidget* m_MyOpenGlWidget;
	QStandardItemModel* tree_model;
	std::shared_ptr<CommandManager> m_CommandManager;
	std::shared_ptr<Document> m_Document;

	void FillTreeModel(const SceneTree& treeNode, QStandardItem* parentItem);
	void ShowNodeProperties(const int nodeIndex);

	EditView* edit_view;
	BuildView* build_view;


};


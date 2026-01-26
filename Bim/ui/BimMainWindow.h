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

#include <vsg/all.h>
#include <vsgQt/Window.h>

#include "MyOpenGLWidget.h"
#include "FileLoadFactory.h"
#include <ui/EditView.h>
#include <ui/BuildView.h>
#include "Document.h"
#include "../Graphics/Mesh.h"
#include "../Scene/Node.h"

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
	vsg::ref_ptr<vsg::StateGroup> CreateNodeFromMesh(const std::shared_ptr<Bim::Graphics::Mesh>& mesh,const std::string& guid);
	vsg::ref_ptr<vsg::Group> CreateVsgNodeFromSceneNode(const std::shared_ptr<Bim::Scene::Node>& node);
	void ShowScene(const std::shared_ptr<Bim::Scene::Node>& node, const vsg::ref_ptr<vsg::Group>& parentGroup);
	vsg::ref_ptr<vsg::StateGroup> CreateGeometryFromMesh(const std::shared_ptr<Bim::Graphics::Mesh>& mesh);
	vsg::ref_ptr<vsg::MatrixTransform> FindTransformByGuid(vsg::Node* rootNode, const std::string& guid);
	void InitGraphicsPipeline();
	//void InitPhongShaderset();
	vsg::ref_ptr<vsg::Group> CreateVsgNodeFromSceneNodeByPhong(const std::shared_ptr<Bim::Scene::Node>& node);
	void ChangeVsgNodeColor(const vsg::ref_ptr<vsg::Node>& vsgNode);
	vsg::ref_ptr<vsg::StateGroup> FindStateGroup(const vsg::ref_ptr<vsg::Node>& vsgNode);
	void DelVsgNode();
public slots:
	void on_fileopen_triggered();
	void on_modelline_triggered(bool flag);
	void on_bspline_triggered(bool flag);
	void on_ball_triggered(bool flag);
	void on_menu_edit_triggered();
	void on_build_triggered();
	void on_delNode_triggered();
private:
	Ui::BimClass ui;
	MyOpenGLWidget* m_MyOpenGlWidget;
	QStandardItemModel* tree_model;
	std::shared_ptr<CommandManager> m_CommandManager;
	std::shared_ptr<Document> m_Document;

	void FillTreeModel(const SceneTree& treeNode, QStandardItem* parentItem);
	void ShowNodeProperties(const int nodeIndex);

	vsgQt::Window* CreateVsgWindow(vsg::ref_ptr<vsgQt::Viewer> viewer, vsg::ref_ptr<vsg::WindowTraits> traits, vsg::ref_ptr<vsg::Node> vsg_scene, QWindow* parent, const QString& title);

	EditView* edit_view;
	BuildView* build_view;

	vsg::ref_ptr<vsg::Group> m_vsgScene;
	vsg::ref_ptr<vsg::Options> m_vsgOptions;
	vsg::ref_ptr<vsgQt::Viewer> m_vsgViewer;
	vsg::ref_ptr<vsg::ShaderStage> m_vertexShader;
	vsg::ref_ptr<vsg::ShaderStage> m_fragmentShader;

	QTimer* m_animationTimer = nullptr;
	vsg::ref_ptr<vsg::MatrixTransform> m_targetTransform;
	vsg::ref_ptr<vsg::BindGraphicsPipeline>m_bindGraphicsPipeline;

	int index = 0;
	bool isDel = false;
};
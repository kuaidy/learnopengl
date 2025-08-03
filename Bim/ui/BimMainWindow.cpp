#include "BimMainWindow.h"

BimMainWindow::BimMainWindow(QWidget* parent,
	std::shared_ptr<CommandManager> commandManager,
	std::shared_ptr<Document> document)
	: QMainWindow(parent),
	m_CommandManager(commandManager),
	m_Document(document)
{
	ui.setupUi(this);
	ui.splitter->setStretchFactor(0, 0);
	ui.splitter->setStretchFactor(1, 1);
	ui.splitter->setStretchFactor(2, 0);

	m_MyOpenGlWidget = new MyOpenGLWidget(ui.frameopengl, m_CommandManager, m_Document);
	// 设置布局
	QVBoxLayout* layout = new QVBoxLayout(ui.frameopengl);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_MyOpenGlWidget);


	QAction* buildAction = new QAction("建模", this);
	connect(buildAction, &QAction::triggered, this, &BimMainWindow::on_build_triggered);
	menuBar()->addAction(buildAction);

	QAction* editAction = new QAction("编辑", this);
	connect(editAction, &QAction::triggered, this, &BimMainWindow::on_menu_edit_triggered);
	menuBar()->addAction(editAction);

}

BimMainWindow::~BimMainWindow()
{
}

void BimMainWindow::OpenModule(string path) {

}

void BimMainWindow::on_fileopen_triggered() {
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("选择模型文件"),
		"",
		tr("模型文件(*.gltf *.glb *.obj *.fbx *.stl *.ifc);;所有文件 (*.*)")
	);
	if (fileName.isEmpty()) return;
	file_loader = FileLoadFactory::Create(fileName.toStdString());
	file_loader->Load(fileName.toStdString());
	m_MyOpenGlWidget->file_loader = file_loader;

	tree_model = new QStandardItemModel();
	tree_model->setHorizontalHeaderLabels(QStringList() << tr("场景"));
	ui.sceneTree->setModel(tree_model);
	//连接选中节点
	connect(ui.sceneTree->selectionModel(), &QItemSelectionModel::currentChanged,
		this, [this](const QModelIndex& current, const QModelIndex&)
		{
			int nodeIndex = current.data(Qt::UserRole + 1).toInt();
			ShowNodeProperties(nodeIndex);
		});
	FillTreeModel(file_loader->scene_tree, nullptr);
}
void BimMainWindow::on_modelline_triggered(bool flag) {
	/*if (flag) {
		m_MyOpenGlWidget->CommandMode = CommandMode::ModelLine;
		m_MyOpenGlWidget->CreateModelLine();
	}
	else {
		m_MyOpenGlWidget->CommandMode = CommandMode::None;
	}*/
}
/// <summary>
/// 绘制b样条曲线
/// </summary>
/// <param name="flag"></param>
void BimMainWindow::on_bspline_triggered(bool flag) {
	/*if (flag) {
		m_MyOpenGlWidget->CommandMode = CommandMode::BSpline;
	}
	else {
		m_MyOpenGlWidget->CommandMode = CommandMode::None;
	}*/
}
/// <summary>
/// 基本图元绘制，球
/// </summary>
/// <param name="flag"></param>
void BimMainWindow::on_ball_triggered(bool flag) {

}

void BimMainWindow::on_build_triggered() {
	build_view = new BuildView(this, this->m_CommandManager, this->m_Document);
	ui.mainToolBar->clear();
	ui.mainToolBar->addWidget(build_view);
}

/// <summary>
/// 编辑
/// </summary>
void BimMainWindow::on_menu_edit_triggered() {
	edit_view = new EditView(this);
	edit_view->command_manager = this->m_CommandManager;
	ui.mainToolBar->clear();
	ui.mainToolBar->addWidget(edit_view);
}

/// <summary>
/// 填充场景树
/// </summary>
/// <param name="treeNode"></param>
/// <param name="parentItem"></param>
void BimMainWindow::FillTreeModel(const SceneTree& treeNode, QStandardItem* parentItem)
{
	QStandardItem* item = new QStandardItem(QString::fromStdString(treeNode.name));
	item->setData(treeNode.nodeIndex, Qt::UserRole + 1);
	if (parentItem) {
		parentItem->appendRow(item);
	}
	else {
		item->setEditable(false);
		tree_model->appendRow(item);
	}
	for (const auto& child : treeNode.children) {
		FillTreeModel(child, item);
	}
}
/// <summary>
/// 显示节点属性
/// </summary>
/// <param name="nodeIndex"></param>
void BimMainWindow::ShowNodeProperties(const int nodeIndex) {
	QtVariantPropertyManager* manager = new QtVariantPropertyManager(this);
	QtVariantEditorFactory* editorFactory = new QtVariantEditorFactory(this);
	//QtTreePropertyBrowser* browser = new QtTreePropertyBrowser(this);
	ui.browserProperty->setFactoryForManager(manager, editorFactory);
	ui.browserProperty->clear();
	std::vector<ElementProperty> selectedProperties;
	std::copy_if(file_loader->element_properties.begin(), file_loader->element_properties.end(), std::back_inserter(selectedProperties), [nodeIndex](const ElementProperty& ep)
		{
			return ep.node_index == nodeIndex;
		});
	for (int i = 0; i < selectedProperties.size(); ++i) {
		QtVariantProperty* property = manager->addProperty(QMetaType::QString, QString::fromStdString(selectedProperties[i].name));
		property->setValue(QString::fromStdString(selectedProperties[i].value));
		ui.browserProperty->addProperty(property);
	}
}
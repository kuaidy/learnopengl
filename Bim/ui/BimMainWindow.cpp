#include "BimMainWindow.h"
#include <vsgXchange/all.h>

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

	//m_MyOpenGlWidget = new MyOpenGLWidget(ui.frameopengl, m_CommandManager, m_Document);

	m_vsgOptions = vsg::Options::create(vsgXchange::all::create());
	m_vsgOptions->fileCache = vsg::getEnv("VSG_FILE_CACHE");
	m_vsgOptions->paths = vsg::getEnvPaths("VSG_FILE_PATH");
	m_vsgOptions->sharedObjects = vsg::SharedObjects::create();

	auto windowTraits = vsg::WindowTraits::create();
	windowTraits->windowTitle = "vsgQt viewer";
	//m_vsgScene = vsg::read_cast<vsg::Node>("C:/Users/kdyonly/Desktop/555.gltf",options);
	m_vsgScene = vsg::Group::create();
	if (!m_vsgScene)
	{
		return ;
	}
	// create the viewer that will manage all the rendering of the views
	m_vsgViewer = vsgQt::Viewer::create();
	
	auto window = CreateVsgWindow(m_vsgViewer, windowTraits, m_vsgScene, nullptr, "First Window");
	
	auto widget = QWidget::createWindowContainer(window, ui.frameopengl);
	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	// 设置布局
	QVBoxLayout* layout = new QVBoxLayout(ui.frameopengl);
	//layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(widget);

	m_vsgViewer->addEventHandler(vsg::CloseHandler::create(m_vsgViewer));
	m_vsgViewer->setInterval(8);
	m_vsgViewer->continuousUpdate = true;
	m_vsgViewer->compile();

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

	//创建一个立方体
	//auto builder = vsg::Builder::create();
	//vsg::GeometryInfo geomInfo;
	//geomInfo.color.set(1.0f, 0.0f, 0.0f, 1.0f);
	//geomInfo.dx.set(2.0f, 0.0f, 0.0f);
	//geomInfo.dy.set(0.0f, 2.0f, 0.0f);
	//geomInfo.dz.set(0.0f, 0.0f, 2.0f);
	//geomInfo.position.set(0.0f, 0.0f, 0.0f);
	//vsg::StateInfo stateInfo;
	//stateInfo.lighting = true;
	//// 创建带默认光照的 StateGroup
	//auto box = builder->createBox(geomInfo);
	//// 添加到场景根节点
	//m_vsgScene->addChild(box);
	//// 重新编译
	//m_vsgViewer->compile();

	auto vsgNode = vsg::read_cast<vsg::Node>(fileName.toStdString(),m_vsgOptions);
	if (vsgNode) {
		m_vsgScene->addChild(vsgNode);
		m_vsgViewer->compile();
	}
	//file_loader = FileLoadFactory::Create(fileName.toStdString());
	//file_loader->Load(fileName.toStdString());
	//m_MyOpenGlWidget->file_loader = file_loader;

	//tree_model = new QStandardItemModel();
	//tree_model->setHorizontalHeaderLabels(QStringList() << tr("场景"));
	//ui.sceneTree->setModel(tree_model);
	////连接选中节点
	//connect(ui.sceneTree->selectionModel(), &QItemSelectionModel::currentChanged,
	//	this, [this](const QModelIndex& current, const QModelIndex&)
	//	{
	//		int nodeIndex = current.data(Qt::UserRole + 1).toInt();
	//		ShowNodeProperties(nodeIndex);
	//	});
	//FillTreeModel(file_loader->scene_tree, nullptr);
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
/// <summary>
/// 创建vsg窗口
/// </summary>
/// <param name="viewer"></param>
/// <param name="traits"></param>
/// <param name="vsg_scene"></param>
/// <param name="parent"></param>
/// <param name="title"></param>
/// <returns></returns>
vsgQt::Window* BimMainWindow::CreateVsgWindow(vsg::ref_ptr<vsgQt::Viewer> viewer, vsg::ref_ptr<vsg::WindowTraits> traits, vsg::ref_ptr<vsg::Node> vsg_scene, QWindow* parent, const QString& title = {})
{
	auto window = new vsgQt::Window(viewer, traits, parent);
	window->setTitle(title);
	window->initializeWindow();
	// if this is the first window to be created, use its device for future window creation.
	if (!traits->device) traits->device = window->windowAdapter->getOrCreateDevice();
	// compute the bounds of the scene graph to help position camera
	vsg::ComputeBounds computeBounds;
	vsg_scene->accept(computeBounds);
	//vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
	vsg::dvec3 center(0, 0, 0);
	//double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
	double radius = 10000;
	double nearFarRatio = 0.1;

	uint32_t width = window->traits->width;
	uint32_t height = window->traits->height;

	vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel(vsg_scene->getObject<vsg::EllipsoidModel>("EllipsoidModel"));
	vsg::ref_ptr<vsg::Camera> camera;
	{
		// set up the camera
		auto lookAt = vsg::LookAt::create(center + vsg::dvec3(0.0, -radius * 3.5, 0.0), center, vsg::dvec3(0.0, 0.0, 1.0));

		vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
		/*if (ellipsoidModel)
		{
			perspective = vsg::EllipsoidPerspective::create(
				lookAt, ellipsoidModel, 30.0,
				static_cast<double>(width) /
				static_cast<double>(height),
				nearFarRatio, false);
		}
		else*/
		{
			perspective = vsg::Perspective::create(
				30.0,
				static_cast<double>(width) /
				static_cast<double>(height),
				nearFarRatio, radius * 4.5);
		}

		camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(VkExtent2D{ width, height }));
	}

	auto trackball = vsg::Trackball::create(camera, ellipsoidModel);
	trackball->addWindow(*window);

	viewer->addEventHandler(trackball);

	auto commandGraph = vsg::createCommandGraphForView(*window, camera, vsg_scene);

	viewer->addRecordAndSubmitTaskAndPresentation({ commandGraph });

	return window;
}
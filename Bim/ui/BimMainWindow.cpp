#include "BimMainWindow.h"
#include <vsgXchange/all.h>

#include "../Engine/GltfFileHandle.h"

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
	windowTraits->swapchainPreferences.imageCount = 2; // 最少双缓冲
	windowTraits->swapchainPreferences.presentMode = VK_PRESENT_MODE_FIFO_KHR; // 避免 tear
	windowTraits->depthFormat = VK_FORMAT_D32_SFLOAT;
	//m_vsgScene = vsg::read_cast<vsg::Node>("C:/Users/kdyonly/Desktop/555.gltf",options);
	m_vsgScene = vsg::Group::create();

	// 1. 强环境光（模拟天光漫反射）
	auto ambient = vsg::AmbientLight::create();
	ambient->color = { 1.0f, 1.0f, 1.0f };
	ambient->intensity = 1.0f; // 关键：足够高

	// 2. 多方向光（模拟间接光反弹）
	auto light1 = vsg::DirectionalLight::create(); // 主光
	light1->direction = { 0.0f, 0.0f, -1.0f };
	light1->intensity = 100.0f;

	auto light2 = vsg::DirectionalLight::create(); // 补光（左）
	light2->direction = { 1.0f, 0.2f, -0.5f };
	light2->intensity = 100.0f;
	light2->color = { 0.9f, 0.9f, 1.0f };

	auto light3 = vsg::DirectionalLight::create(); // 补光（右）
	light3->direction = { -1.0f, 0.2f, -0.5f };
	light3->intensity = 100.0f;
	light3->color = { 1.0f, 0.9f, 0.9f };

	m_vsgScene->addChild(ambient);
	//m_vsgScene->addChild(light1);
	//m_vsgScene->addChild(light2);
	//m_vsgScene->addChild(light3);

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

	InitGraphicsPipeline();
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

	Bim::Engine::GltfFileHandle gltfFileHandle;
	gltfFileHandle.ReadFile(fileName.toStdString());

	auto& ctx = Bim::Engine::GetGlobalContext();
	ShowScene(ctx.scene, m_vsgScene);
	m_vsgViewer->compile();
	//移动模型
	//m_targetTransform = FindTransformByGuid(m_vsgScene, "Transform_Object_2");
	//if (m_targetTransform)
	//{
	//	// 创建并启动动画定时器（例如每 50ms 更新一次 ≈ 20 FPS）
	//	m_animationTimer = new QTimer(this);
	//	connect(m_animationTimer, &QTimer::timeout, this, [this]()
	//		{
	//			static double x = 0.0;
	//			x += 0.1; // 每次向右移动 0.1 单位
	//			m_targetTransform->matrix = vsg::translate(x, 0.0, 0.0);
	//		});
	//	m_animationTimer->start(20); // 50 毫秒间隔
	//}

	//auto vsgNode = vsg::read_cast<vsg::Node>(fileName.toStdString(), m_vsgOptions);
	//if (vsgNode) {
	//	m_vsgScene->addChild(vsgNode);
	//	m_vsgViewer->compile();
	//}
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


void BimMainWindow::on_delNode_triggered()
{
	DelVsgNode();
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
	double radius = 3000;
	double nearPlane = 0.1;
	double farPlane = 50000;

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
				nearPlane, farPlane);
		}

		camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(VkExtent2D{ width, height }));
	}

	auto trackball = vsg::Trackball::create(camera, ellipsoidModel);
	trackball->addWindow(*window);

	viewer->addEventHandler(trackball);

	auto renderGraph = vsg::RenderGraph::create(window->windowAdapter);
	VkClearValue colorClear{};
	colorClear.color = { {0.2f, 0.2f, 0.2f, 1.0f} };
	VkClearValue depthClear{};
	depthClear.depthStencil = { 1.0f, 0 };  // 深度 = 1.0
	renderGraph->clearValues = { colorClear };

	auto commandGraph = vsg::createCommandGraphForView(*window, camera, vsg_scene);
	//commandGraph->addChild(renderGraph);
	viewer->addRecordAndSubmitTaskAndPresentation({ commandGraph });

	return window;
}

vsg::ref_ptr<vsg::StateGroup> BimMainWindow::CreateNodeFromMesh(const std::shared_ptr<Bim::Graphics::Mesh>& mesh, const std::string& guid) {
	if (!mesh) {
		return nullptr;
	}

	if (mesh->vertices.empty()) {
		std::cerr << "警告: 顶点数据为空" << std::endl;
		return nullptr;
	}

	if (mesh->vertices.size() % 3 != 0) {
		std::cerr << "错误: 顶点数据大小不是 3 的倍数" << std::endl;
		return nullptr;
	}

	if (mesh->indices.empty()) {
		std::cerr << "警告: 索引数据为空" << std::endl;
		return nullptr;
	}
	// 安全前提
	static_assert(sizeof(vsg::vec3) == 3 * sizeof(float));
	size_t numVertices = mesh->vertices.size() / 3;
	auto vertices = vsg::vec3Array::create(numVertices);
	std::memcpy(vertices->data(), mesh->vertices.data(), mesh->vertices.size() * sizeof(float));

	vsg::ref_ptr<vsg::vec3Array> normals = nullptr;
	if (!mesh->normals.empty()) {
		size_t numNormals = mesh->normals.size() / 3;
		auto normals = vsg::vec3Array::create(numNormals);
		std::memcpy(normals->data(), mesh->normals.data(), mesh->normals.size() * sizeof(float));
	}

	auto indices = vsg::uintArray::create(mesh->indices.size());
	std::copy(mesh->indices.begin(), mesh->indices.end(), indices->begin());

	auto vid = vsg::VertexIndexDraw::create();
	vid->assignArrays(vsg::DataList{ vertices });
	vid->assignIndices(indices);
	vid->indexCount = static_cast<uint32_t>(indices->size());
	vid->instanceCount = 1;
	vid->setValue("guid", guid);



	// create texture image and associated DescriptorSets and binding
	//auto texture = vsg::DescriptorImage::create(vsg::Sampler::create(), textureData, 0, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

	//auto uniformValue = vsg::vec3Value::create(1.0f, 2.0f, 3.0f);
	//auto uniform = vsg::DescriptorBuffer::create(uniformValue, 1, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

	//auto descriptorSet = vsg::DescriptorSet::create(descriptorSetLayout, vsg::Descriptors{ texture, uniform });
	//auto bindDescriptorSets = vsg::BindDescriptorSets::create(VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->layout, 0, vsg::DescriptorSets{ descriptorSet });

	// create StateGroup as the root of the scene/command graph to hold the GraphicsPipeline, and binding of Descriptors to decorate the whole graph
	auto scenegraph = vsg::StateGroup::create();
	scenegraph->add(m_bindGraphicsPipeline);
	//scenegraph->add(bindDescriptorSets);

	// set up model transformation node
	auto transform = vsg::MatrixTransform::create(); // VK_SHADER_STAGE_VERTEX_BIT
	transform->setValue("guid", std::string("Transform_" + guid));
	// add transform to root of the scene graph

	// setup geometry
	//auto drawCommands = vsg::Commands::create();
	//drawCommands->addChild(vsg::BindVertexBuffers::create(0, vsg::DataList{ vertices }));
	//drawCommands->addChild(vsg::BindIndexBuffer::create(indices));
	//drawCommands->addChild(vsg::DrawIndexed::create(indices->size(), 1, 0, 0, 0));

	// add drawCommands to transform
	transform->addChild(vid);
	scenegraph->addChild(transform);
	return scenegraph;
}

void BimMainWindow::ShowScene(const std::shared_ptr<Bim::Scene::Node>& node, const vsg::ref_ptr<vsg::Group>& parentGroup)
{
	//if (node->model) {
	//	for (const auto& mesh : node->model->meshes) {
	//		auto vsgNode = CreateNodeFromMesh(mesh, node->guid);
	//		if (vsgNode) {
	//			m_vsgScene->addChild(vsgNode);
	//		}
	//	}
	//}
	//auto vsgNode = CreateVsgNodeFromSceneNode(node);
	auto vsgNode = CreateVsgNodeFromSceneNodeByPhong(node);
	if (vsgNode)
	{
		parentGroup->addChild(vsgNode);
		ChangeVsgNodeColor(vsgNode);
	}
	for (const auto& childNode : node->children) {
		ShowScene(childNode, vsgNode);
	}

}


vsg::ref_ptr<vsg::StateGroup> BimMainWindow::CreateGeometryFromMesh(const std::shared_ptr<Bim::Graphics::Mesh>& mesh)
{
	return nullptr;
	//if (!mesh || mesh->vertices.empty() || mesh->indices.empty()) return {};

	//auto geometry = vsg::Geometry::create();

	//// 顶点 (position)
	//size_t numVertices = mesh->vertices.size() / 3;
	//auto vertices = vsg::vec3Array::create(numVertices);
	//std::memcpy(vertices->data(), mesh->vertices.data(), mesh->vertices.size() * sizeof(float));

	//// 法线（可选）
	//vsg::ref_ptr<vsg::vec3Array> normals = nullptr;
	//if (!mesh->normals.empty() && mesh->normals.size() / 3 == numVertices) {
	//	normals = vsg::vec3Array::create(numVertices);
	//	std::memcpy(normals->data(), mesh->normals.data(), mesh->normals.size() * sizeof(float));
	//}

	//// 索引
	//auto indices = vsg::uintArray::create(mesh->indices.size());
	//std::copy(mesh->indices.begin(), mesh->indices.end(), indices->begin());

	//// 绑定到 Geometry
	//geometry->arrays.push_back(vertices);
	//if (normals) geometry->arrays.push_back(normals);
	//geometry->indices = indices;

	//// 设置图元类型（通常是三角形）
	//geometry->commands.push_back(vsg::DrawIndexed::create(
	//	static_cast<uint32_t>(indices->size()), 1, 0, 0, 0
	//));


	//// 2. 直接从 VSG 内置路径加载 simple 着色器
	//auto vertexShader = vsg::ShaderStage::read(VK_SHADER_STAGE_VERTEX_BIT, "main", "shaders/simple.vert", m_vsgOptions);
	//auto fragmentShader = vsg::ShaderStage::read(VK_SHADER_STAGE_FRAGMENT_BIT, "main", "shaders/simple.frag", m_vsgOptions);

	//if (!vertexShader || !fragmentShader) {
	//	std::cerr << "Failed to load simple shaders!" << std::endl;
	//	return nullptr;
	//}

	//// 3. 手动创建最简管线（无需材质）
	//vsg::VertexInputState::Bindings vertexBindings{
	//	VkVertexInputBindingDescription{0, sizeof(vsg::vec3), VK_VERTEX_INPUT_RATE_VERTEX}
	//};
	//vsg::VertexInputState::Attributes vertexAttributes{
	//	VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0} // location=0 → position
	//};


	//// 1. 创建空的 DescriptorSetLayout 列表（因为不需要纹理/UBO）
	//vsg::DescriptorSetLayouts descriptorSetLayouts; // 空 vector

	//// 2. 定义 push constant range
	//vsg::PushConstantRanges pushConstantRanges{
	//	VkPushConstantRange{VK_SHADER_STAGE_VERTEX_BIT, 0, 128}
	//};

	//// 3. 创建 PipelineLayout
	//auto pipelineLayout = vsg::PipelineLayout::create(descriptorSetLayouts, pushConstantRanges);

	//auto graphicsPipeline = vsg::GraphicsPipeline::create(
	//	pipelineLayout,
	//	vsg::ShaderStages{ vertexShader, fragmentShader },
	//	vsg::GraphicsPipelineStates{
	//		vsg::VertexInputState::create(vertexBindings, vertexAttributes),
	//		vsg::InputAssemblyState::create(),
	//		vsg::RasterizationState::create(),
	//		vsg::MultisampleState::create(),
	//		vsg::ColorBlendState::create(),      // 默认 blend 关闭，直接输出颜色
	//		vsg::DepthStencilState::create()     // 启用 depth test
	//	}
	//);

	//auto bindPipeline = vsg::BindGraphicsPipeline::create(graphicsPipeline);

	//// 4. 组装场景节点
	//auto stategroup = vsg::StateGroup::create();
	//stategroup->add(bindPipeline);
	//stategroup->addChild(geometry);

	//return stategroup;

	//return geometry;
}

vsg::ref_ptr<vsg::MatrixTransform> BimMainWindow::FindTransformByGuid(vsg::Node* rootNode, const std::string& guid)
{
	struct Visitor : public vsg::Visitor
	{
		const std::string& targetGuid;
		vsg::ref_ptr<vsg::MatrixTransform> found;

		Visitor(const std::string& guid) : targetGuid(guid) {}

		void apply(vsg::Object& object) override
		{
			// 只检查 MatrixTransform 类型
			if (auto transform = object.cast<vsg::MatrixTransform>())
			{
				std::string nodeGuid;
				if (transform->getValue("guid", nodeGuid) && nodeGuid == targetGuid)
				{
					found = transform;
					return; // 找到即停（若需找多个则继续）
				}
			}

			// 继续遍历子节点
			object.traverse(*this);
		}
	};

	Visitor visitor(guid);
	rootNode->accept(visitor);
	return visitor.found;
}

/// <summary>
/// 初始化渲染管线
/// </summary>
void BimMainWindow::InitGraphicsPipeline()
{
	auto vertexShader = vsg::ShaderStage::read(VK_SHADER_STAGE_VERTEX_BIT, "main", "Shaders/base.vert", m_vsgOptions);
	auto fragmentShader = vsg::ShaderStage::read(VK_SHADER_STAGE_FRAGMENT_BIT, "main", "Shaders/base.frag", m_vsgOptions);

	if (!vertexShader || !fragmentShader) {
		std::cerr << "Failed to load simple shaders!" << std::endl;
		return;
	}

	vsg::PushConstantRanges pushConstantRanges{
		{VK_SHADER_STAGE_VERTEX_BIT, 0, 128}
	};

	vsg::VertexInputState::Bindings vertexBindingsDescriptions{
		VkVertexInputBindingDescription{0, sizeof(vsg::vec3), VK_VERTEX_INPUT_RATE_VERTEX}, // vertex data
		VkVertexInputBindingDescription{1, sizeof(vsg::vec3), VK_VERTEX_INPUT_RATE_VERTEX}, // normal data
		VkVertexInputBindingDescription{2, sizeof(vsg::vec4), VK_VERTEX_INPUT_RATE_VERTEX}  // color data
	};

	vsg::VertexInputState::Attributes vertexAttributeDescriptions{
		VkVertexInputAttributeDescription{0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}, // vertex data
		VkVertexInputAttributeDescription{1, 1, VK_FORMAT_R32G32B32_SFLOAT, 0}, // normal data
		VkVertexInputAttributeDescription{2, 2, VK_FORMAT_R32G32B32_SFLOAT, 0}, // color data
	};

	auto rasterizationState = vsg::RasterizationState::create();
	rasterizationState->cullMode = VK_CULL_MODE_NONE;
	//rasterizationState->cullMode = VK_CULL_MODE_NONE;
	//rasterizationState->frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	auto depth = vsg::DepthStencilState::create();
	//depth->depthTestEnable = VK_TRUE;
	//depth->depthWriteEnable = VK_TRUE;
	//depth->depthCompareOp = VK_COMPARE_OP_LESS;

	vsg::GraphicsPipelineStates pipelineStates{
		vsg::VertexInputState::create(vertexBindingsDescriptions, vertexAttributeDescriptions),
		vsg::InputAssemblyState::create(),
		rasterizationState,
		vsg::MultisampleState::create(),
		vsg::ColorBlendState::create(),
		depth };

	auto pipelineLayout = vsg::PipelineLayout::create(vsg::DescriptorSetLayouts{ }, pushConstantRanges);
	auto graphicsPipeline = vsg::GraphicsPipeline::create(pipelineLayout, vsg::ShaderStages{ vertexShader, fragmentShader }, pipelineStates);
	m_bindGraphicsPipeline = vsg::BindGraphicsPipeline::create(graphicsPipeline);
}
/// <summary>
/// 从场景节点创建VSG节点
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
vsg::ref_ptr<vsg::Group> BimMainWindow::CreateVsgNodeFromSceneNode(const std::shared_ptr<Bim::Scene::Node>& node)
{
	auto transform = vsg::MatrixTransform::create();
	auto guidValue = vsg::Value<std::string>::create(std::string("Transform_") + node->guid);
	transform->setValue("guid", guidValue);
	if (node->localMatrix.isZero())
	{
		transform->matrix = vsg::dmat4(); // 单位阵
	}
	else
	{
		transform->matrix = vsg::dmat4(node->localMatrix.data());
	}
	if (node->model)
	{
		auto group = vsg::Group::create();
		group->setValue("guid", node->guid);
		for (auto& geometry : node->model->geometries)
		{
			if (geometry->mesh) {
				auto& mesh = geometry->mesh;
				if (mesh->vertices.empty()) {
					std::cerr << "警告: 顶点数据为空" << std::endl;
					continue;
				}
				if (mesh->vertices.size() % 3 != 0) {
					std::cerr << "错误: 顶点数据大小不是 3 的倍数" << std::endl;
					continue;
				}
				if (mesh->indices.empty()) {
					std::cerr << "警告: 索引数据为空" << std::endl;
					continue;
				}
				//顶点
				size_t numVertices = mesh->vertices.size() / 3;
				if (numVertices < 3) continue;
				auto vertices = vsg::vec3Array::create(numVertices);
				std::memcpy(vertices->data(), mesh->vertices.data(), mesh->vertices.size() * sizeof(float));
				//法线
				vsg::ref_ptr<vsg::vec3Array> normals = nullptr;
				if (!mesh->normals.empty()) {
					size_t numNormals = mesh->normals.size() / 3;
					normals = vsg::vec3Array::create(numNormals);
					std::memcpy(normals->data(), mesh->normals.data(), mesh->normals.size() * sizeof(float));
				}
				//索引
				auto indices = vsg::uintArray::create(mesh->indices.size());
				std::copy(mesh->indices.begin(), mesh->indices.end(), indices->begin());

				//顶点颜色
				vsg::ref_ptr<vsg::vec4Array> colors = nullptr;
				if (!mesh->colors.empty()) {
					size_t numColors = mesh->colors.size() / 4;
					colors = vsg::vec4Array::create(numColors);
					std::memcpy(colors->data(), mesh->colors.data(), mesh->colors.size() * sizeof(float));
				}

				auto stateGroup = vsg::StateGroup::create();
				stateGroup->add(m_bindGraphicsPipeline);

				//材质
				/*if (geometry->material)
				{
					vsg::vec4 color(static_cast<float>(geometry->material->baseColorFactor.x()),
						static_cast<float>(geometry->material->baseColorFactor.y()),
						static_cast<float>(geometry->material->baseColorFactor.z()),
						static_cast<float>(geometry->material->baseColorFactor.w()));
					auto colorData = vsg::vec4Value::create(color);
					auto material = vsg::PushConstants::create(
						VK_SHADER_STAGE_FRAGMENT_BIT,
						0,
						colorData
					);
					stateGroup->add(material);
				}*/

				vsg::DataList arrays{ vertices };
				if (normals) arrays.push_back(normals);
				if (colors) arrays.push_back(colors);

				auto vid = vsg::VertexIndexDraw::create();
				vid->assignArrays(arrays);
				vid->assignIndices(indices);
				vid->indexCount = static_cast<uint32_t>(indices->size());
				vid->instanceCount = 1;
				stateGroup->addChild(vid);

				group->addChild(stateGroup);
			}
		}
		transform->addChild(group);
	}
	return transform;
}

/// <summary>
///	初始化Phong着色器集
/// </summary>
//void BimMainWindow::InitPhongShaderset()
//{
	//// 1. 内置 Phong Shader
	//auto shaderSet = vsg::createPhongShaderSet(options);

	//// 2. 材质
	//auto material = vsg::PhongMaterial::create();
	//material->diffuse = vsg::vec4(0.8f, 0.6f, 0.2f, 1.0f);
	//material->ambient = vsg::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	//material->specular = vsg::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//material->shininess = 32.0f;

	//// 3. descriptor set
	//auto descriptorSetLayout = shaderSet->descriptorSetLayouts.front();

	//auto descriptorSet = vsg::DescriptorSet::create(
	//	descriptorSetLayout,
	//	{
	//		vsg::DescriptorBuffer::create(material, 0)
	//	}
	//);

	//// 4. pipeline layout
	//auto pipelineLayout = vsg::PipelineLayout::create(
	//	{ descriptorSetLayout },
	//	shaderSet->pushConstantRanges
	//);

	//vsg::GraphicsPipelineConfig pipelineConfig(shaderSet);
	//pipelineConfig.assignDescriptors = false;

	//auto pipeline = pipelineConfig.createPipeline(pipelineLayout);

	//// 5. state group
	//auto stateGroup = vsg::StateGroup::create();
	//stateGroup->add(pipeline);

	//stateGroup->add(
	//	vsg::BindDescriptorSet::create(
	//		VK_PIPELINE_BIND_POINT_GRAPHICS,
	//		pipelineLayout,
	//		0,
	//		descriptorSet
	//	)
	//);

	//// 6. 添加geometry
	//stateGroup->addChild(geometry);

	//return stateGroup;
//}


/// <summary>
/// 从场景节点创建VSG节点，使用vsg phone着色器
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
vsg::ref_ptr<vsg::Group> BimMainWindow::CreateVsgNodeFromSceneNodeByPhong(const std::shared_ptr<Bim::Scene::Node>& node)
{
	auto scenegraph = vsg::Group::create();

	//auto sharedObjects = vsg::SharedObjects::create();

	auto shaderSet = vsg::createPhongShaderSet(m_vsgOptions);
	//auto shaderSet = vsg::createFlatShadedShaderSet(m_vsgOptions);
	//绘制成线模型
	//auto rasterizationState = vsg::RasterizationState::create();
	//rasterizationState->polygonMode = VK_POLYGON_MODE_LINE;
	//rasterizationState->cullMode = VK_CULL_MODE_NONE;
	//graphicsPipelineConfig->pipelineStates.push_back(rasterizationState);

	if (node->model)
	{
		for (auto& geometry : node->model->geometries)
		{
			if (geometry->mesh)
			{
				auto graphicsPipelineConfig = vsg::GraphicsPipelineConfigurator::create(shaderSet);
				auto mat = vsg::PhongMaterialValue::create();
				mat->value().ambient.set(0.5, 0.5, 0.5, 1.0);   // 非常高的环境光反射
				mat->value().diffuse.set(0.5, 0.5, 0.5, 1.0);   // 极低的漫反射
				mat->value().specular.set(0.0, 0.0, 0.0, 1.0);  // 无镜面
				mat->value().shininess = 1.0f;
				graphicsPipelineConfig->assignDescriptor("material", mat);

				auto& mesh = geometry->mesh;
				//顶点
				static_assert(sizeof(vsg::vec3) == 3 * sizeof(float));
				size_t numVertices = mesh->vertices.size() / 3;
				auto vertices = vsg::vec3Array::create(numVertices);
				std::memcpy(vertices->data(), mesh->vertices.data(), mesh->vertices.size() * sizeof(float));
				//法向量
				vsg::ref_ptr<vsg::vec3Array> normals = nullptr;
				if (!mesh->normals.empty()) {
					size_t numNormals = mesh->normals.size() / 3;
					normals = vsg::vec3Array::create(numNormals);
					std::memcpy(normals->data(), mesh->normals.data(), mesh->normals.size() * sizeof(float));
				}
				//顶点颜色
				vsg::ref_ptr<vsg::vec4Array> colors = nullptr;
				if (!mesh->colors.empty()) {
					size_t numColors = mesh->colors.size() / 4;
					colors = vsg::vec4Array::create(numColors);
					std::memcpy(colors->data(), mesh->colors.data(), mesh->colors.size() * sizeof(float));
				}
				vsg::DataList vertexArrays;
				graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, vertices);
				if (normals) {
					graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, normals);
				}
				//graphicsPipelineConfig->assignArray(vertexArrays, "vsg_TexCoord0", VK_VERTEX_INPUT_RATE_VERTEX, texcoords);
				graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_INSTANCE, colors);

				//索引
				auto indices = vsg::uintArray::create(mesh->indices.size());
				std::copy(mesh->indices.begin(), mesh->indices.end(), indices->begin());

				// setup geometry
				auto drawCommands = vsg::Commands::create();
				drawCommands->addChild(vsg::BindVertexBuffers::create(graphicsPipelineConfig->baseAttributeBinding, vertexArrays));
				drawCommands->addChild(vsg::BindIndexBuffer::create(indices));
				drawCommands->addChild(vsg::DrawIndexed::create(indices->size(), 1, 0, 0, 0));

				//if (sharedObjects) sharedObjects->share(vertexArrays);
				//if (sharedObjects) sharedObjects->share(indices);

				//if (sharedObjects)
				//{
				//	sharedObjects->share(drawCommands->children);
				//	sharedObjects->share(drawCommands);
				//}
				//// share the pipeline config and initialize it if it's unique
				//if (sharedObjects)
				//	sharedObjects->share(graphicsPipelineConfig, [](auto gpc) { gpc->init(); });
				//else

				auto rasterState = vsg::RasterizationState::create();
				rasterState->cullMode = VK_CULL_MODE_NONE; // 关闭剔除
				graphicsPipelineConfig->pipelineStates.push_back(rasterState);

				graphicsPipelineConfig->init();
				auto stateGroup = vsg::StateGroup::create();

	/*			auto descriptorSet = graphicsPipelineConfig->descriptorConfigurator->descriptorSets[1];
				stateGroup->add(vsg::BindDescriptorSet::create(
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					graphicsPipelineConfig->layout,
					0,
					descriptorSet
				));*/

				graphicsPipelineConfig->copyTo(stateGroup);




				// set up model transformation node
				/*auto transform = vsg::MatrixTransform::create();
				transform->subgraphRequiresLocalFrustum = false;*/
				auto transform = vsg::MatrixTransform::create();
				transform->setValue("guid", "Transform_" + node->guid);
				if (node->localMatrix.isZero())
				{
					transform->matrix = vsg::dmat4(); // 单位阵
				}
				else
				{
					transform->matrix = vsg::dmat4(node->localMatrix.data());
				}
				stateGroup->addChild(drawCommands);
				/*	if (sharedObjects)
					{
						sharedObjects->share(stateGroup);
					}*/
				transform->addChild(stateGroup);
				/*if (sharedObjects)
				{
					sharedObjects->share(transform);
				}*/
				scenegraph->addChild(transform);
			}
		}
	}
	return scenegraph;
}

void BimMainWindow::ChangeVsgNodeColor(const vsg::ref_ptr<vsg::Node>& vsgNode)
{
	//auto stateGroup = FindStateGroup(vsgNode);
	//if (!stateGroup) return;
	//vsg::ref_ptr<vsg::PhongMaterialValue> material;
	//// 遍历所有 StateCommand，寻找 Descriptor
	//for (auto& sc : stateGroup->stateCommands)
	//{
	//	auto bindDS = sc->cast<vsg::BindDescriptorSet>();
	//	if (!bindDS) continue;

	//	for (auto& d : bindDS->descriptorSet->descriptors)
	//	{
	//		// 找 binding=0 的材质
	//		if (d->dstBinding != 0) continue;

	//		// d 的真实类型是 DescriptorBuffer
	//		auto descBuf = d->cast<vsg::DescriptorBuffer>();
	//		if (!descBuf) continue;

	//		if (descBuf->bufferInfoList.empty()) continue;

	//		auto info = descBuf->bufferInfoList.front();
	//		if (!info || !info->data) continue;

	//		material = dynamic_cast<vsg::PhongMaterialValue*>(info->data.get());
	//	}
	//}

	//// 如果没有材质，则创建
	//if (!material)
	//{
	//	material = vsg::PhongMaterialValue::create();
	//	material->value().diffuse.set(1.0f, 0.0f, 0.0f, 1.0f);
	//	material->value().specular.set(1.0f, 1.0f, 1.0f, 1.0f);
	//	material->value().shininess = 32.0f;

	//	auto bufferInfo = vsg::BufferInfo::create(material);
	//	auto descBuf = vsg::DescriptorBuffer::create(bufferInfo, 0); // binding = 0
	//	stateGroup->add(descBuf);
	//	return;
	//}
	//material->value().diffuse.set(1.0f, 0.0f, 0.0f, 1.0f); // 蓝色主体
}

vsg::ref_ptr<vsg::StateGroup> BimMainWindow::FindStateGroup(const vsg::ref_ptr<vsg::Node>& vsgNode)
{
	if (auto sg = vsgNode.cast<vsg::StateGroup>(); sg)
		return sg;

	if (auto group = vsgNode.cast<vsg::Group>(); group) {
		for (auto& child : group->children) {
			if (auto sg = FindStateGroup(child); sg)
				return sg;
		}
	}

	return {};
}

void BimMainWindow::DelVsgNode()
{
	for (int i = m_vsgScene->children.size() - 1; i >= 0; --i) {
		if (!isDel) {
			m_vsgScene->children.erase(m_vsgScene->children.begin() + i);
			isDel = true;
		}
	}
}

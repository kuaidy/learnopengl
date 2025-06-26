阶段一：建模基础理论与 API 实践（1~1.5 月）
📚 几何基础
齐次坐标、变换矩阵、四元数

向量运算、射线与几何体交互

点/边/面表示，Mesh 拓扑结构

📘 实践路线
熟悉 OpenGL + ImGui + GLM + Assimp

建立 Mesh 数据结构，绘制线框、多边形网格

加入鼠标旋转、平移、缩放交互（Arcball）

推荐资源：
LearnOpenGL

《3D Math Primer for Graphics and Game Development》

OpenMesh（处理拓扑）

阶段二：建模核心算法与几何处理（2~3 月）
🧠 重点算法（建议用 C++ 实现）：
布尔运算（CSG）：

基于 BSP Tree 或 Mesh 重建（GJK/Weiler–Atherton）

网格操作：

Euler 操作（加点/删边/三角剖分/合并面）

Laplacian 平滑、重建法线

曲线/曲面建模：

Bézier 曲线、B-Spline、NURBS 理论与实践

实体建模：

拉伸（extrude）、旋转（revolve）、扫掠（sweep）

推荐库：
CGAL：高质量几何算法库

OpenCascade（工业级 CAD/建模内核）

LibIGL（轻量级几何处理库）

阶段三：项目实战 + 面试准备（1.5~2 月）
🎯 实战项目建议：
自研一个简易建模系统（支持以下操作）：

绘制草图（2D sketch）

拉伸体、旋转体、布尔差集

实时预览与网格高亮交互

导入导出 STL/OBJ

项目展示：

加入 ImGui 面板（操作参数、布尔类型等）

可视化 BVH / AABB 包围盒 / 法线 / 网格线框

🧩 面试准备内容：
类别	示例题目
几何算法	如何计算两 Mesh 的布尔运算？
Mesh 数据结构	Half-edge / Winged-edge 结构作用？
实体建模	旋转体怎么生成？如何保证闭合？
可视化	如何高效选中网格上的一个面？
性能	如何加速布尔运算或点选？
实践	你怎么实现过倒角（Fillet）？使用了什么数据结构？

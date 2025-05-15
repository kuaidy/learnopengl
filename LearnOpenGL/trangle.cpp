#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

//窗口改变的时候，视口跟随调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//输入
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}



int maintrangle()
{
	if (!glfwInit()) return -1;

	//设置 opengl 上下文版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //次版本
	//启用核心配置文件，禁用旧版固定管线功能
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
	if (!window) {
		std::cerr << "failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//绘制三角形
	//顶点着色器
	//const char* vertexShaderSource = "#version 460 core\n"
	//	"layout (location=0) in vec3 aPos;\n"
	//	"void main()\n"
	//	"{\n"
	//	"	gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
	//	"}\0";
	//unsigned int vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//glCompileShader(vertexShader);
	////检测是否编译成功
	//int success;
	//char infoLog[512];
	//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//if (!success) {
	//	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	//	std::cout << "vertexshader compilation failed" << infoLog << std::endl;
	//}

	////片段着色器
	//const char* fragmentShaderSource = "#version 330 core\n"
	//	"out vec4 FragColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	//	"}\n\0";
	//unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);;
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);
	//glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	//if (!success) {
	//	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	//	std::cout << "fragmentshader compilation failed" << infoLog << std::endl;
	//}

	////着色器程序
	//unsigned int shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, fragmentShader);
	//glLinkProgram(shaderProgram);
	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	//if (!success) {
	//	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	//	std::cout << "link program failed" << infoLog << std::endl;
	//}

	////绿色着色器
	//const char* fragmentShaderGreenSource = R"(
	//	#version 460 core
	//	out vec4 FragColor;
	//	void main(){
	//		FragColor=vec4(0.0f,1.0f,0.0f,1.0f);
	//	}
	//)";
	//unsigned int fragmentShaderGreen = glCreateShader(GL_FRAGMENT_SHADER);;
	//glShaderSource(fragmentShaderGreen, 1, &fragmentShaderGreenSource, NULL);
	//glCompileShader(fragmentShaderGreen);
	//glGetShaderiv(fragmentShaderGreen, GL_COMPILE_STATUS, &success);
	//if (!success) {
	//	glGetShaderInfoLog(fragmentShaderGreen, 512, NULL, infoLog);
	//	std::cout << "fragmentshader compilation failed" << infoLog << std::endl;
	//}

	////着色器程序
	//unsigned int shaderProgramGreen = glCreateProgram();
	//glAttachShader(shaderProgramGreen, vertexShader);
	//glAttachShader(shaderProgramGreen, fragmentShaderGreen);
	//glLinkProgram(shaderProgramGreen);
	//glGetProgramiv(shaderProgramGreen, GL_LINK_STATUS, &success);
	//if (!success) {
	//	glGetProgramInfoLog(shaderProgramGreen, 512, NULL, infoLog);
	//	std::cout << "link program failed" << infoLog << std::endl;
	//}

	////uniform 着色器
	//const char* fragmentShaderUniformSource = R"(
	//	#version 460 core
	//	out vec4 FragColor;
	//	uniform vec4 ourColor;
	//	void main(){
	//		FragColor=ourColor;
	//	}
	//)";
	//unsigned int fragmentShaderUniform = glCreateShader(GL_FRAGMENT_SHADER);;
	//glShaderSource(fragmentShaderUniform, 1, &fragmentShaderUniformSource, NULL);
	//glCompileShader(fragmentShaderUniform);
	//glGetShaderiv(fragmentShaderUniform, GL_COMPILE_STATUS, &success);
	//if (!success) {
	//	glGetShaderInfoLog(fragmentShaderGreen, 512, NULL, infoLog);
	//	std::cout << "fragmentshader compilation failed" << infoLog << std::endl;
	//}
	////uniform 着色器程序
	//unsigned int shaderProgramUniform = glCreateProgram();
	//glAttachShader(shaderProgramUniform, vertexShader);
	//glAttachShader(shaderProgramUniform, fragmentShaderUniform);
	//glLinkProgram(shaderProgramUniform);
	//glGetProgramiv(shaderProgramUniform, GL_LINK_STATUS, &success);
	//if (!success) {
	//	glGetProgramInfoLog(shaderProgramGreen, 512, NULL, infoLog);
	//	std::cout << "link program failed" << infoLog << std::endl;
	//}

	////删除着色器
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);
	//glDeleteShader(fragmentShaderGreen);
	//glDeleteShader(shaderProgramUniform);

	////顶点数组对象
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	 0.5f, -0.5f, 0.0f,
	//	 0.0f,  0.5f, 0.0f,
	//	 //0.0f,  -0.5, 0.0f,
	//	 //-0.5f,0.5f,0.0f,
	//	 //0.5f,0.5f,0.0f
	//};

	////顶点数组对象
	//float vertices1[] = {
	//	 0.0f,  -0.5, 0.0f,
	//	 -0.5f,0.5f,0.0f,
	//	 0.5f,0.5f,0.0f
	//};

	////创建顶点数组对象
	////unsigned int VBO, VAO;
	////glGenVertexArrays(1, &VAO);
	////glBindVertexArray(VAO);
	////glGenBuffers(1, &VBO);
	////glBindBuffer(GL_ARRAY_BUFFER, VBO);
	////glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	////glEnableVertexAttribArray(0);

	////创建多个顶点数据对象
	//unsigned int VAOs[2], VBOs[2];
	//glGenVertexArrays(2, VAOs);
	//glGenBuffers(2, VBOs);

	//glBindVertexArray(VAOs[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);


	//glBindVertexArray(VAOs[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);


	int success;
	char infoLog[512];

	//顶点着色器
	const char* vertexColorShaderSource = R"(
		#version 460 core
		layout (location=0) in vec3 aPos;
		layout (location=1) in vec3 aColor;
		out vec3 ourColor;
		void main(){
			gl_Position=vec4(aPos,1.0);
			ourColor=aColor;
		}
	)";
	//通过代码创建一个顶点着色器
	unsigned int vertexColorShader = glCreateShader(GL_VERTEX_SHADER);
	//着色器源码
	glShaderSource(vertexColorShader, 1, &vertexColorShaderSource, NULL);
	//编译着色器
	glCompileShader(vertexColorShader);
	//判断着色器是否编译成功
	glGetShaderiv(vertexColorShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexColorShader, 512, NULL, infoLog);
		std::cerr << "failed compile vertexColorShader:" << infoLog << std::endl;
		return -1;
	}
	//片段着色器
	const char* fragmentColorShaderSource = R"(
		#version 460 core
		out vec4 FragColor;
		in vec3 ourColor;
		void main(){
			FragColor=vec4(ourColor,1.0f);
		}
	)";
	unsigned int fragmentColorShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentColorShader, 1, &fragmentColorShaderSource, NULL);
	glCompileShader(fragmentColorShader);
	glGetShaderiv(fragmentColorShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentColorShader, 512, NULL, infoLog);
		std::cerr << "failed compile fragmentColorShader:" << infoLog << std::endl;
		return -1;
	}
	//着色器程序
	//1.创建一个着色器程序
	unsigned int colorShaderProgram = glCreateProgram();
	//2.添加着色器
	glAttachShader(colorShaderProgram, vertexColorShader);
	glAttachShader(colorShaderProgram,fragmentColorShader);
	//3.连接着色器程序
	glLinkProgram(colorShaderProgram);
	//4.判断链接错误信息
	glGetProgramiv(colorShaderProgram,GL_LINK_STATUS,&success);
	if (!success)
	{
		glGetProgramInfoLog(colorShaderProgram, 512, NULL, infoLog);
		std::cerr << "failed link colorShaderProgram:" << infoLog << std::endl;
		return -1;
	}
	//带颜色的顶点数组
	float verticeColors[] =
	{
		// 位置          // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticeColors), verticeColors, GL_STATIC_DRAW);
	//确定位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//使用着色器程序
		/*glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgramGreen);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/
		//uniform
		//glUseProgram(shaderProgramUniform);
		////更新uniform颜色
		//float timeValue = glfwGetTime();
		//float colorValue = sin(timeValue) / 2.0f + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgramUniform, "ourColor");
		//glUniform4f(vertexColorLocation, colorValue, colorValue, 1.0f, 1.0f);
		//glBindVertexArray(VAOs[0]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//使用着色器程序
		glUseProgram(colorShaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteVertexArrays(2, VAOs);
	//glDeleteBuffers(2, VBOs);
	//glDeleteProgram(shaderProgram);
	//glDeleteProgram(shaderProgramGreen);
	glDeleteProgram(colorShaderProgram);


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"
#include "camera.h"
#include <vector>
#include <map>

using namespace std;

//������ͼ
unsigned int loadTexture(char const* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format= GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture faild to load at path:" << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


int main() {
	if (!glfwInit()) {
		std::cerr << "init failed" << std::endl;
		return -1;
	}
	//�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//����������
	GLFWwindow* window = glfwCreateWindow(800, 600, "learn opengl", NULL, NULL);
	if (!window) {
		std::cerr << "failed create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//�����Ļ����󶨵���ǰ����
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//������Ȳ���,z����
	glEnable(GL_DEPTH_TEST);
	//���û��
	glEnable(GL_BLEND);
	//�������
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//�������޳�
	glEnable(GL_CULL_FACE);
	//ֻ�޳�����
	glCullFace(GL_BACK);
	//����ǰ��Ϊ��ʱ��
	glFrontFace(GL_CW);

	//float cubeVertices[] = {
	//	// positions          // texture Coords
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	//};

	//���޳��Ķ������ݣ���ʱ������
	//float cubeVertices[] = {
	//	// Back face
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	//	// Front face
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	//	// Left face
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	//	// Right face
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
	//	 // Bottom face
	//	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	//	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
	//	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	//	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	//	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	//	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	//	 // Top face
	//	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	//	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	//	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
	//	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	//	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	//	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	//};

	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 // Bottom face          
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		 // Top face
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	//����
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	//ȷ��λ������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//��ͼ
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//�ذ�
	float planeVertices[] = {
		// positions          // texture Coords 
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	unsigned int floorVAO, floorVBO;
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glBindVertexArray(floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	//λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ͼ
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//͸����ͼ��С��
	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	vector<glm::vec3> vegetation
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};
	unsigned int transparentVAO, transparentVBO;
	glGenVertexArrays(1, &transparentVAO);
	glGenBuffers(1, &transparentVBO);
	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	//λ��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//��ͼ
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//������ͼ
	unsigned int cubeTexture = loadTexture("./Images/container2.png");
	unsigned int floorTexture = loadTexture("./Images/marble.jpg");
	//unsigned int grassTexture = loadTexture("./Images/grass.png");
	unsigned int transparentTexture = loadTexture("./Images/blending_transparent_window.png");

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 viewPos(0.0f, 0.0f, 5.0f);
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor(1.0f, 0.0f, 1.0f);

	//����
	Shader objShader("./Shaders/20250602-1.vs", "./Shaders/20250602-1.fs");
	Camera camera(viewPos);

	objShader.use();
	objShader.setInt("texture1", 0);

	while (!glfwWindowShouldClose(window))
	{
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		objShader.setMat4("view", view);
		objShader.setMat4("projection", projection);

		//��������
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		objShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//���Ƶذ�
		glBindVertexArray(floorVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		model = glm::mat4(1.0f);
		objShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		////����С��
		//glBindVertexArray(transparentVAO);
		//glBindTexture(GL_TEXTURE_2D, grassTexture);
		//for (unsigned int i = 0; i < vegetation.size(); i++) {
		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, vegetation[i]);
		//	objShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		//���ƴ���
		//���մ�Զ������˳�����͸������
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			float distance = glm::length(camera.Position-vegetation[i]);
			sorted[distance] = vegetation[i];
		}

		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, transparentTexture);
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			objShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//������Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1,&floorVAO);
	glDeleteVertexArrays(1,&transparentVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1,&floorVBO);
	glDeleteBuffers(1,&transparentVBO);
	objShader.del();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
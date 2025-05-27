#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"
#include "camera.h"

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

	//�з������Ķ�������
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//ȷ��λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//��ͼ
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//ȷ��λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//������Ȳ���,z����
	glEnable(GL_DEPTH_TEST);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 viewPos(0.0f, 0.0f, 5.0f);
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor(1.0f, 0.0f, 1.0f);

	//����
	Shader objShader("./Shaders/20250527.vs", "./Shaders/20250527.fs");
	Shader lightShader("./Shaders/20250527.vs", "./Shaders/lightfragment.fs");
	Camera camera(viewPos);

	//��ͼ
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//���أ�����������
	int width, height, nrChannels;
	unsigned char* data = stbi_load("./Images/container2.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//���ӱ߿������
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//���أ�����������
	data = stbi_load("./Images/container2_specular.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//���ӷ������ͼ
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//���أ�����������
	data = stbi_load("./Images/matrix.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	while (!glfwWindowShouldClose(window))
	{
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objShader.use();
		//�������
		objShader.setInt("material.diffuse", 0);
		objShader.setInt("material.specular", 1);
		//objShader.setInt("material.emission", 2);
		objShader.setFloat("material.shininess", 32.0f);

		//����
		objShader.setVec3("light.position", camera.Position);
		//objShader.setVec3("light.direction",-0.2f,-1.0f,-0.3);//ƽ�й�
		objShader.setVec3("light.direction",camera.Front);
		objShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		objShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5)));
		objShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		objShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // �����յ�����һЩ�Դ��䳡��
		objShader.setVec3("light.specular", 1.0f, 0.0f, 0.0f);
		//�����Դ
		objShader.setFloat("light.constant", 1.0f);
		objShader.setFloat("light.linear", 0.09f);
		objShader.setFloat("light.quadratic", 0.032f);

		//�����
		objShader.setVec3("dirLight.direction", camera.Front);
		objShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		objShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f); // �����յ�����һЩ�Դ��䳡��
		objShader.setVec3("dirLight.specular", 1.0f, 0.0f, 0.0f);

		//���Դ
		// point light 1
		objShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		objShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		objShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		objShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		objShader.setFloat("pointLights[0].constant", 1.0f);
		objShader.setFloat("pointLights[0].linear", 0.09f);
		objShader.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		objShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		objShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		objShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		objShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		objShader.setFloat("pointLights[1].constant", 1.0f);
		objShader.setFloat("pointLights[1].linear", 0.09f);
		objShader.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		objShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		objShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		objShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		objShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		objShader.setFloat("pointLights[2].constant", 1.0f);
		objShader.setFloat("pointLights[2].linear", 0.09f);
		objShader.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		objShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		objShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		objShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		objShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		objShader.setFloat("pointLights[3].constant", 1.0f);
		objShader.setFloat("pointLights[3].linear", 0.09f);
		objShader.setFloat("pointLights[3].quadratic", 0.032f);

		//�۹�
		objShader.setVec3("spotLight.position", camera.Position);
		objShader.setVec3("spotLight.direction", camera.Front);
		objShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		objShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5)));
		objShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
		objShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f); // �����յ�����һЩ�Դ��䳡��
		objShader.setVec3("spotLight.specular", 1.0f, 0.0f, 0.0f);
		//�����Դ
		objShader.setFloat("spotLight.constant", 1.0f);
		objShader.setFloat("spotLight.linear", 0.09f);
		objShader.setFloat("spotLight.quadratic", 0.032f);

		// �ڴ�֮ǰ��Ҫ�������� use ��Ӧ����ɫ���������趨uniform��
		objShader.setVec3("viewPos", camera.Position);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		objShader.setMat4("model", model);

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		objShader.setMat4("view", view);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		objShader.setMat4("projection", projection);

		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			objShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//lightShader.use();
		//glm::mat4 lightModel = glm::mat4(1.0f);
		//lightModel = glm::translate(lightModel, lightPos);
		//lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		//lightShader.setMat4("model", lightModel);
		//lightShader.setMat4("view", view);
		//lightShader.setMat4("projection", projection);

		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//������Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	objShader.del();
	lightShader.del();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
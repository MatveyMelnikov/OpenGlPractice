#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ResourceManager.h"

const char *vertex_shader_src =
"#version 460\n"
"layout (location = 0) in vec3 vertex_position;"
"uniform mat4 modelMatrix;"
"uniform mat4 viewMatrix;"
"out vec3 color;"
"void main() {"
"	gl_Position = viewMatrix * modelMatrix * vec4(vertex_position, 1.0);"
"}";

const char *fragment_shader_src =
"#version 460\n"
"uniform vec3 color;"
"out vec4 fragColor;"
"void main() {"
"	fragColor = vec4(color, 1.0);"
"}";

const int CAMERA_SPEED = 1.0;
const glm::ivec2 WINDOW_SIZE = glm::ivec2(640, 480);

void render(GLFWwindow* window, int width, int height) {
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width, height);
	ResourceManager* resourceManager = ResourceManager::getInstance();

	// Holding the camera in the center of the window
	/*resourceManager->setCameraPosition(
		glm::ivec2((WINDOW_SIZE.x - width) / 2.f, (WINDOW_SIZE.y - height) / 2.f)
	);*/

	resourceManager->render(glm::ivec2(width, height));

	glfwSwapBuffers(window);
}

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "Window size: " << width << " " << height << std::endl;
	render(window, width, height);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	/*if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;*/ //��� ��� ����� ���������������

	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_W:
			ResourceManager::getInstance()->moveCamera(glm::fvec2(0.f, CAMERA_SPEED));
			break;
		case GLFW_KEY_A:
			ResourceManager::getInstance()->moveCamera(glm::fvec2(-CAMERA_SPEED, 0.f));
			break;
		case GLFW_KEY_D:
			ResourceManager::getInstance()->moveCamera(glm::fvec2(CAMERA_SPEED, 0.f));
			break;
		case GLFW_KEY_S:
			ResourceManager::getInstance()->moveCamera(glm::fvec2(0.f, -CAMERA_SPEED));
			break;
		case GLFW_KEY_E:
			break;
		case GLFW_KEY_Q:
			break;
		default:
			break;
		}
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) // and �� �������� 
		glfwSetWindowShouldClose(window, 1); // ������������� ���� ������ � �������
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cerr << "ERROR: GLFW wasnt create!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Network", NULL, NULL);
	if (!window)
	{
		std::cerr << "ERROR: Window wasnt create!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		std::cerr << "ERROR: Glad wasnt init!" << std::endl;
		return -1;
	}

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);

	ResourceManager* resourceManager = ResourceManager::getInstance();
	resourceManager->setCameraOffset(glm::fvec2(320, 240));
	resourceManager->createShaderProgram(vertex_shader_src, fragment_shader_src);
	resourceManager->createCircle(
		glm::ivec2(0, 0),
		10,
		glm::fvec3(1.f, 0.f, 0.f),
		0
	);
	/*resourceManager->createCircle(
		glm::ivec2(100, 100),
		10,
		glm::fvec3(1.f, 0.f, 0.f),
		0
	);*/
	resourceManager->createLine(
		glm::ivec2(0, 0),
		glm::ivec2(100, 100),
		30,
		glm::fvec3(0.f, 0.f, 1.f),
		0
	);

	resourceManager->moveCamera(glm::ivec2(100, 100));

	while (!glfwWindowShouldClose(window))
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		render(window, width, height);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine.h"
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

const glm::ivec2 WINDOW_SIZE = glm::ivec2(640, 480);

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	// Rendering during screen resizing
	Engine* engine = Engine::getInstance(window);
	engine->render();
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

	Engine* engine = Engine::getInstance(window);
	ResourceManager* resourceManager = ResourceManager::getInstance();
	resourceManager->createShaderProgram(vertex_shader_src, fragment_shader_src);
	resourceManager->createCircle(
		glm::ivec2(0, 0),
		10,
		glm::fvec3(1.f, 0.f, 0.f),
		0
	);
	resourceManager->createCircle(
		glm::ivec2(300, 200),
		10,
		glm::fvec3(1.f, 0.f, 0.f),
		0
	);
	resourceManager->createLine(
		glm::ivec2(0, 0),
		glm::ivec2(300, 200),
		10,
		glm::fvec3(0.f, 0.f, 1.f),
		0
	);

	while (!glfwWindowShouldClose(window))
	{
		engine->render();
	}

	glfwTerminate();
	return 0;
}
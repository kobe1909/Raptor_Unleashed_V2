#include "App.h"
#include "glm/gtc/matrix_transform.hpp"

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " in " << file << " on " << line << std::endl;
		return false;
	}

	return true;
}



bool App::CreateWindow(glm::vec2 windowSize, const char* title) {
	this->windowSize = windowSize;
	proj = glm::perspective(45.f, windowSize.x / windowSize.y, 0.01f, 50.f);

	if (!glfwInit())
		return false;

	window = glfwCreateWindow(this->windowSize.x, this->windowSize.y, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glewInit();
	//GLCALL(glClearColor(1, 0, 0, 1));
	GLCALL(glEnable(GL_DEPTH_TEST));

	return true;
}

void App::Run(std::function<void(double)> fun) {
	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		if (width != windowSize.x || height != windowSize.y) {
			windowSize = glm::vec2(width, height);
			GLCALL(glViewport(0, 0, width, height));
			proj = glm::perspective(45.f, windowSize.x / windowSize.y, 0.01f, 50.f);
		}

		GLCALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
		double current_time = glfwGetTime();
		double deltaTime = current_time - lastTime;
		lastTime = current_time;

		fun(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
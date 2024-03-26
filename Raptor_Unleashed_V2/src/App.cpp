#include "App.h"

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



bool App::CreateWindow(int width, int height, const char* title) {
	if (!glfwInit())
		return false;

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glClearColor(1, 0, 0, 1);

	return true;
}

void App::Run(std::function<void(double)> fun) {
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		double current_time = glfwGetTime();
		double deltaTime = current_time - lastTime;
		lastTime = current_time;

		fun(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
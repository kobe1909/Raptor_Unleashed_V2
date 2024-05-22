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



bool App::GetKeyState(int key, int state) {
	return glfwGetKey(window, key) == state;
}
bool App::GetKeyState(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool App::CreateWindow(glm::vec2 windowSize, const char* title, bool maximize) {
	this->windowSize = windowSize;
	if (!glfwInit())
		return false;

	window = glfwCreateWindow(this->windowSize.x, this->windowSize.y, title, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glewInit();
	GLCALL(glClearColor(1, 0, 0, 1));
	GLCALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
	GLCALL(glEnable(GL_DEPTH_TEST));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	if (maximize) {
		glfwMaximizeWindow(window);
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		windowSize = glm::vec2(width, height);
	}

	proj = glm::perspective(45.f, windowSize.x / windowSize.y, 0.01f, 50.f);

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
			delete[] buffer;
			buffer = new int[width, height];
		}

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		mousePos = glm::vec2((float)xPos, (float)yPos);

		if (firstMouseMove) {
			lastMousePos = mousePos;
			firstMouseMove = false;
		}

		mouseOffset = mousePos - lastMousePos;
		lastMousePos = mousePos;

		if (GetKeyState(GLFW_KEY_LEFT_ALT, GLFW_PRESS)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstMouseMove = true;
		}		

		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		GLCALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		double current_time = glfwGetTime();
		double deltaTime = current_time - lastTime;
		lastTime = current_time;

		fun(deltaTime);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		// Save the window buffer to a file
		//GLCALL(glReadPixels(0, 0, windowSize.x, windowSize.y, GL_BGR, GL_UNSIGNED_BYTE, buffer));

		glfwPollEvents();
	}
}

void App::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
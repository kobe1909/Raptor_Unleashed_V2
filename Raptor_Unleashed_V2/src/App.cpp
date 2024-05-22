#include "App.h"
#include "glm/gtc/matrix_transform.hpp"
#include "lodepng/lodepng.h"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(this->windowSize.x, this->windowSize.y, title, NULL, NULL);
	buffer = new unsigned char[windowSize.x * windowSize.y * 4];
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
			delete buffer;
			buffer = new unsigned char[width * height * 4];
			GLCALL(glViewport(0, 0, width, height));
			proj = glm::perspective(45.f, windowSize.x / windowSize.y, 0.01f, 50.f);
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
		std::cout << "Clear" << std::endl;
		GLCALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		double current_time = glfwGetTime();
		double deltaTime = current_time - lastTime;
		lastTime = current_time;

		glfwSetWindowTitle(window, ("Raptor Unleashed " + std::to_string(1 / deltaTime)).c_str());
		std::cout << "Main loop begon" << std::endl;
		fun(deltaTime);
		std::cout << "Main loop ended" << std::endl;

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		bool safeToImage = false;
		if (safeToImage) {
			GLCALL(glReadPixels(0, 0, windowSize.x, windowSize.y, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
			std::cout << "Got pixel buffer" << std::endl;

			// put the pixels into a vector 
			std::vector<unsigned char> image;
			for (int i = 0; i < windowSize.x * windowSize.y * 4; i++) {
				unsigned char data = *(buffer + i);

				image.push_back(data);
			}
			std::cout << "Created image vector" << std::endl;


			//Encode the image
			unsigned error = lodepng::encode("testframe.png", image, windowSize.x, windowSize.y);
			std::cout << "Saved into image" << std::endl;

			//if there's an error, display it
			if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		}

		glfwPollEvents();
	}
}

void App::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
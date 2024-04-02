#pragma once

#include "Transform.h"
#include <iostream>

class Camera : public Transform {
public:
	glm::mat4 view;

	Camera(glm::vec3 position, glm::vec3 rotation) : view(glm::mat4(0.f)) {
		this->position = position;
		this->rotation = rotation;
		this->scale = glm::vec3(1.f, 1.f, 1.f);
		
		view = GetView();
	}

	glm::mat4 GetView() {
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 lookDir;
		lookDir.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		lookDir.y = sin(glm::radians(rotation.x));
		lookDir.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		std::cout << rotation.x << "\t" << rotation.y << "\t" << rotation.z << "\t" << std::endl;

		return glm::lookAt(position, position + lookDir, up);
	}
};
#pragma once

#include "Transform.h"

class Camera : public Transform {
public:
	glm::mat4 view;

	Camera(glm::vec3 position, glm::vec3 rotation) : view(glm::mat4(0.f)) {
		this->position = position;
		this->rotation = rotation;
		this->scale = glm::vec3(1.f, 1.f, 1.f);
		
		glm::vec3 target = glm::vec3(0, 0, 0);
		glm::vec3 up = glm::vec3(0, 1, 0);

		view = glm::lookAt(position, target, up);
	}
};
#pragma once

#include "Transform.h"
#include <iostream>

class Camera : public Transform {
public:
	Camera(glm::vec3 position, glm::vec3 rotation) {
		this->position = position;
		this->rotation = rotation;
		this->scale = glm::vec3(1.f, 1.f, 1.f);
	}

	void SetTransform(Transform transform) {
		this->position = transform.position;
		this->rotation = transform.rotation;
		this->scale    = transform.scale;
	}

	glm::vec3 GetUpVector() {
		return glm::vec3(0, 1, 0);
	}

	glm::vec3 GetFrontVector() {
		glm::vec3 lookDir;
		lookDir.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		lookDir.y = sin(glm::radians(rotation.x));
		lookDir.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		return glm::normalize(lookDir);
	}

	glm::mat4 GetView() {
		return glm::lookAt(position, position + GetFrontVector(), GetUpVector());
	}
};
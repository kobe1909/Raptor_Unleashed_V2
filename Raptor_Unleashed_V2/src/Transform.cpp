#include "Transform.h"

Transform::Transform() : position(), rotation(), scale(glm::vec3(1.0f, 1.0f, 1.0f)) {}

Transform::Transform(glm::vec3 n_position, glm::vec3 n_rotation, glm::vec3 n_scale)
	: position(n_position), rotation(n_rotation), scale(n_scale) {}

glm::mat4 Transform::GetModelMatrix() {
	glm::mat4 matrix = glm::mat4(1.f);

	// Translate the matrix
	matrix = glm::translate(matrix, position);

	// Rotate the matrix
	// x
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));

	// y
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));

	// z 
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));

	// Apply Scale
	matrix = glm::scale(matrix, scale);

	// Return matrix
	return matrix;
}
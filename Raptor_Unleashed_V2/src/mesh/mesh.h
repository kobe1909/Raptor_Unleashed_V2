#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "../shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh {
public:
	// Mesh data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);

private:
	unsigned int arrayObject, bufferObject, indexObject;

	void setupMesh();
};
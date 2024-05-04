#include "mesh.h"
#include "App.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices  = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::setupMesh() {
	GLCALL(glGenVertexArrays(1, &arrayObject));
	GLCALL(glGenBuffers(1, &bufferObject));
	GLCALL(glGenBuffers(1, &indexObject));

	GLCALL(glBindVertexArray(arrayObject));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, bufferObject));

	GLCALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexObject));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	// Vertex positions
	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

	// Vertex normals
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

	// Vertex texture coords
	GLCALL(glEnableVertexAttribArray(2));
	GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

	GLCALL(glBindVertexArray(0));
}

void Mesh::Draw(Shader &shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		GLCALL(glActiveTexture(GL_TEXTURE0 + i));

		std::string number;
		std::string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);

		shader.SetUniform1i(("material." + name + number).c_str(), i);
		GLCALL(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}
	glActiveTexture(GL_TEXTURE0);
	shader.SetUniform1f("material.shininess", 32.0f);

	// Draw mesh
	GLCALL(glBindVertexArray(arrayObject));
	GLCALL(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
	GLCALL(glBindVertexArray(0));
}
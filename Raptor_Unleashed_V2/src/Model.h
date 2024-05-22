#pragma once

#include "shader.h"
#include "mesh.h"
#include <vector>
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model {
public:
	// Model data
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;
	bool flipTexture;

	Model(std::string const &path, bool flipTextures = true, bool gamma = false) : gammaCorrection(gamma), flipTexture(flipTextures) {
		loadModel(path);
	}

	void Draw(Shader& shader);

private:
	void loadModel(std::string const &path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);
};


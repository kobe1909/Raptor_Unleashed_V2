#pragma once
#include "shader.h"
#include "glm/glm.hpp"
#include <string>
#include <iostream>

enum LightType {
	None = 0, Directional = 1, Point = 2, Spot = 3
};

class Light {
public:
	LightType lightType = LightType::None;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	virtual void AddToShader(Shader& shader, std::string uniformBaseName) = 0;
};

class DirectionalLight : public Light {
public:
	glm::vec3 direction;

	DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		lightType = LightType::Directional;
		this->direction = direction;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	void AddToShader(Shader& shader, std::string uniformBaseName) {
		shader.SetUniformVec3((uniformBaseName + ".direction").c_str(), direction);
		shader.SetUniformVec3((uniformBaseName + ".ambient").c_str(), ambient);
		shader.SetUniformVec3((uniformBaseName + ".diffuse").c_str(), diffuse);
		shader.SetUniformVec3((uniformBaseName + ".specular").c_str(), specular);
	}
};

class PointLight : public Light {
public:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;

	PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {
		lightType = LightType::Point;
		this->position = position;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
	}

	void AddToShader(Shader& shader, std::string uniformBaseName) {
		shader.SetUniformVec3((uniformBaseName + ".position").c_str(), position);
		shader.SetUniformVec3((uniformBaseName + ".ambient").c_str(), ambient);
		shader.SetUniformVec3((uniformBaseName + ".diffuse").c_str(), diffuse);
		shader.SetUniformVec3((uniformBaseName + ".specular").c_str(), specular);
		shader.SetUniform1f((uniformBaseName + ".constant").c_str(), constant);
		shader.SetUniform1f((uniformBaseName + ".Linear").c_str(), linear);
		shader.SetUniform1f((uniformBaseName + ".quadratic").c_str(), quadratic);
	}
};

class SpotLight : public Light {
	glm::vec3 position;
	glm::vec3 direction;
	float innerCutOff;
	float outerCutOff;

	SpotLight(glm::vec3 position, glm::vec3 direction, float innerCutOff, float outerCutOff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		lightType = LightType::Spot;
		this->position = position;
		this->direction = direction;
		this->innerCutOff = innerCutOff;
		this->outerCutOff = outerCutOff;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	void AddToShader(Shader& shader, std::string uniformBaseName) {
		shader.SetUniformVec3((uniformBaseName + ".position").c_str(), position);
		shader.SetUniformVec3((uniformBaseName + ".direction").c_str(), direction);
		shader.SetUniform1f((uniformBaseName + ".innerCutOff").c_str(), innerCutOff);
		shader.SetUniform1f((uniformBaseName + ".outerCutOff").c_str(), outerCutOff);
		shader.SetUniformVec3((uniformBaseName + ".ambient").c_str(), ambient);
		shader.SetUniformVec3((uniformBaseName + ".diffuse").c_str(), diffuse);
		shader.SetUniformVec3((uniformBaseName + ".specular").c_str(), specular);
	}
};
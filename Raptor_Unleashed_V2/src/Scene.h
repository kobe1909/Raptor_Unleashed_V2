#pragma once

#include "BaseComponent.h"
#include <vector>
#include "Light.h"
#include "Transform.h"
#include "Camera.h"

class Scene {
public:
	std::vector<BaseComponent*> objects = {};
	std::vector<Light*> lights = {};
	Camera camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	void Register(BaseComponent*);
	void Register(std::vector<BaseComponent*>);

	void AddLight(Light*);
	void AddLight(std::vector<Light*>);

	void AddLightsToShader(Shader& shader);
	void AddCameraToShader(Shader& shader);

	void Start();
	void Update(double deltaTime, App& app, Scene& scene);
	void Destroy();
};


#pragma once

#include "BaseComponent.h"
#include <vector>
#include "Light.h"

class Scene {
public:
	std::vector<BaseComponent*> objects = {};
	std::vector<Light*> lights = {};

	void Register(BaseComponent*);
	void Register(std::vector<BaseComponent*>);

	void AddLight(Light*);
	void AddLight(std::vector<Light*>);

	void AddLightsToShader(Shader& shader);

	void Start();
	void Update(double deltaTime, App& app, Scene& scene);
	void Destroy();
};


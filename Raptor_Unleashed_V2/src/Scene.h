#pragma once

#include "BaseComponent.h"
#include <vector>
#include "Light.h"
#include "Transform.h"
#include "Camera.h"
#include "App.h"

class Scene {
public:
	Scene(App* app) { this->app = app;  }
	Scene(App* app, std::vector<BaseComponent*>, std::vector<Light*>, Camera&);
	std::vector<BaseComponent*> objects = {};
	std::vector<Light*> lights = {};
	Camera camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	App* app;

	void Register(BaseComponent*);
	void Register(std::vector<BaseComponent*>);

	void AddLight(Light*);
	void AddLight(std::vector<Light*>);

	void AddLightsToShader(Shader& shader);
	void AddCameraToShader(Shader& shader);
	void AddSceneToShader(Shader& shader);

	template<class T>
	T GetObjectByName(std::string name);

	void Start();
	void Update(double deltaTime);
	void Draw();
	void Destroy();
};


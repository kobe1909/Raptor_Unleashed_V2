#include "Scene.h"
#include "App.h"
#include <list>
#include <stdexcept>

Scene::Scene(App* app, std::vector<BaseComponent*> objects, std::vector<Light*> lights, Camera* camera) {
	this->app = app;
	Register(objects);
	AddLight(lights);
	this->camera = camera;
}

void Scene::Register(BaseComponent* object) {
	for (auto& element : objects) {
		if (element->name == object->name) {
			throw std::invalid_argument("An object with this name already exists in this scene.");
		}
	}
	object->scene = this;
	objects.push_back(object);
	object->scene = this;
	object->app = app;
}
void Scene::Register(std::vector<BaseComponent*> new_objects) {
	for (auto& element : new_objects) {
		Register(element);
	}
}

void Scene::AddLight(Light* light) {
	lights.push_back(light);
}
void Scene::AddLight(std::vector<Light*> new_lights) {
	for (auto& element : new_lights) {
		lights.push_back(element);
	}
}

void Scene::AddLightsToShader(Shader& shader) {
	int nPointLights = 0;
	for (Light*& light : lights) {
		if (light->lightType == LightType::Directional) {
			light->AddToShader(shader, "dirLight");
		}
		else if (light->lightType == LightType::Point) {
			light->AddToShader(shader, "pointLights[" + std::to_string(nPointLights) + "]");
			nPointLights++;
		}
		else if (light->lightType == LightType::Spot) {
			light->AddToShader(shader, "spotLight");
		}
	}
}
void Scene::AddCameraToShader(Shader& shader) {
	shader.SetUniformMat4f("view", camera->GetView());
}

void Scene::AddSceneToShader(Shader& shader) {
	shader.Bind();
	AddLightsToShader(shader);
	AddCameraToShader(shader);
	shader.SetUniformMat4f("proj", app->proj);
}

void Scene::Start() {
	for (auto& element : objects) {
		element->OnStart();
	}
}

void Scene::Update(double deltaTime) {
	for (auto& element : objects) {
		element->OnUpdate(deltaTime);
	}
}

void Scene::Draw() {
	for (auto& element : objects) {
		element->OnDraw();
	}
}

void Scene::Destroy() {
	for (auto& element : objects) {
		element->OnDestroy();
	}
}
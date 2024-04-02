#include "Scene.h"
#include "App.h"

Scene::Scene(std::vector<BaseComponent*> objects, std::vector<Light*> lights, Camera& camera) {
	this->objects = objects;
	this->lights = lights;
	this->camera = camera;
}

void Scene::Register(BaseComponent* object) {
	objects.push_back(object);
}
void Scene::Register(std::vector<BaseComponent*> new_objects) {
	for (auto& element : new_objects) {
		objects.push_back(element);
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
	shader.SetUniformMat4f("view", camera.GetView());
}

void Scene::Start() {
	for (auto& element : objects) {
		element->OnStart();
	}
}

void Scene::Update(double deltaTime, App& app, Scene& scene) {
	for (auto& element : objects) {
		element->OnUpdate(deltaTime, app, scene);
	}
}

void Scene::Destroy() {
	for (auto& element : objects) {
		element->OnDestroy();
	}
}
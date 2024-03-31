#include "Scene.h"
#include "App.h"

void Scene::Register(BaseComponent* object) {
	objects.push_back(object);
}
void Scene::Register(std::vector<BaseComponent*> new_objects) {
	for (auto& element : new_objects) {
		objects.push_back(element);
	}
}


void Scene::Start() {
	for (auto& element : objects) {
		element->OnStart();
	}
}

void Scene::Update(double deltaTime, App app) {
	for (auto& element : objects) {
		element->OnUpdate(deltaTime, app);
	}
}

void Scene::Destroy() {
	for (auto& element : objects) {
		element->OnDestroy();
	}
}
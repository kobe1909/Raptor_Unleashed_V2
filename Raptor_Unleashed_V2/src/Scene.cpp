#include "Scene.h"

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

void Scene::Update() {
	for (auto& element : objects) {
		element->OnUpdate();
	}
}

void Scene::Destroy() {
	for (auto& element : objects) {
		element->OnDestroy();
	}
}
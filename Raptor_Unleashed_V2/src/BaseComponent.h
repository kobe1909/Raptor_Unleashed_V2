#pragma once
#include <string>
#include "Transform.h"
#include "shader.h"
#include "App.h"

// Forward declaration to avoid cycloid includes
class Scene;

class BaseComponent {
public:
	Transform transform = Transform();
	Shader shader = Shader("res/shaders/Camera.hlsl");
	Scene* scene = nullptr;
	App* app = nullptr;
	std::string name = "";

	virtual void OnStart() = 0;
	virtual void OnUpdate(double deltaTime) = 0;
	virtual void OnDraw() = 0;
	virtual void OnDestroy() = 0;

	operator BaseComponent* () { return this; }
};

/*
* class Entity : public BaseComponent {
*	additional variables
*	
*	void OnStart() {
*		this code will be run at the start of the game
*	}
*	void OnUpdate() {
*		this code will run every frame
*	}
*	void OnDestroy() {
*		this code will run at the end of the game
*	}
* }
*/
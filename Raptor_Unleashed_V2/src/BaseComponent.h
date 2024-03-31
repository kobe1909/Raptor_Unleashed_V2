#pragma once
#include "Transform.h"
#include "shader.h"
#include "App.h"

class BaseComponent {
public:
	Transform transform = Transform();
	Shader shader = Shader("res/shaders/Camera.hlsl");

	virtual void OnStart() = 0;
	virtual void OnUpdate(double deltaTime, App app) = 0;
	virtual void OnDestroy() = 0;
};
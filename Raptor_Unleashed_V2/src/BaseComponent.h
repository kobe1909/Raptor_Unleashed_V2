#pragma once
#include "Transform.h"
#include "shader.h"

class BaseComponent {
public:
	Transform transform = Transform();
	Shader shader = Shader("res/shaders/Camera.hlsl");

	virtual void OnStart() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDestroy() = 0;
};
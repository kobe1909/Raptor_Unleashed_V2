#pragma once
#include <string>
#include "Transform.h"
#include "shader.h"
#include "Model.h"

// Forward declaration to avoid cycloid includes
class Scene;
class App;

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

	void Draw(Model& model) {
		shader.SetUniformMat4f("model", transform.GetModelMatrix());
		model.Draw(shader);
	}

	operator BaseComponent* () { return this; }
};

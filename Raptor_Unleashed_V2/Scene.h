#pragma once

#include "BaseComponent.h"
#include <vector>

class Scene {
public:
	std::vector<BaseComponent*> objects = {};

	void Register(BaseComponent*);

	void Start();
	void Update();
	void Destroy();
};


#pragma once

class BaseComponent {
public:
	int test = 10;
	virtual void OnStart() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDestroy() = 0;
};
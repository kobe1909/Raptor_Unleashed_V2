#pragma once

#include "LayoutObject.h"

class ArrayObject {
private:
	unsigned int m_renderID;

public:
	ArrayObject();
	~ArrayObject();

	void AddBuffer(const BufferObject& vb, const LayoutObject& layout);

	void Bind() const;
	void UnBind() const;
};
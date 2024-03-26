#pragma once

#include "../App.h"
#include <vector>

#include "BufferObject.h"
#include "ArrayObject.h"
#include "IndexObject.h"
#include "LayoutObject.h"

class Mesh { 
private:
	BufferObject m_bufferObject;
	IndexObject  m_indexObject;
	ArrayObject  m_arrayObject;

public:
	Mesh(
		const void* vertices, 
		unsigned int verticesSize, 
		LayoutObject& layout, 
		const unsigned int* triangleData, 
		unsigned int triangleCount
	);

	unsigned int GetTriangleCount();

	void Bind() const;
	void UnBind() const;
};


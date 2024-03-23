#pragma once

#include "App.h"
#include <vector>

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


class BufferObject {
private:
	unsigned int m_renderID;

public:
	BufferObject(const void* data, unsigned int size);
	~BufferObject();

	void Bind() const;
	void UnBind() const;
};

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

class IndexObject {
private:
	unsigned int m_renderID;
	unsigned int m_count;

public:
	IndexObject(const unsigned int* data, unsigned int count);
	~IndexObject();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_count; }
};

struct BufferElement {
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT:         return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		// Stop program running if type isn't in the list
		ASSERT(false);
		return 0;
	}
};

class LayoutObject {
private:
	std::vector<BufferElement> m_elements;
	unsigned int m_stride;

public:
	LayoutObject() : m_stride(0) {}

	template<typename T>
	void Push(unsigned int count) {
		// Raise compiler error when the type isn't implemented
		static_assert(true);
	}

	template<>
	void Push<float>(unsigned int count) {
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += BufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += BufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_stride += BufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<BufferElement> GetElements() const& { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }
};


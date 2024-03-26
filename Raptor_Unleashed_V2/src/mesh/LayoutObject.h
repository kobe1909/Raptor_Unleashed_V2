#pragma once

#include "../App.h"
#include <vector>

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

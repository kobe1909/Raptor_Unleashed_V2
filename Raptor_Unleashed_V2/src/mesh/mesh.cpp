#include "mesh.h"
#include "LayoutObject.h"
#include "IndexObject.h"
#include "ArrayObject.h"
#include "BufferObject.h"

Mesh::Mesh(
	const void* vertices, 
	unsigned int verticesSize, 
	LayoutObject& layout, 
	const unsigned int* triangleData, 
	unsigned int triangleCount
) : m_bufferObject(vertices, verticesSize), 
	m_indexObject(triangleData, triangleCount),
	m_arrayObject() 
{
	m_arrayObject.AddBuffer(m_bufferObject, layout);
}

unsigned int Mesh::GetTriangleCount() {
	return m_indexObject.GetCount();
}

void Mesh::Bind() const {
	m_arrayObject.Bind();
	m_bufferObject.Bind();
	m_indexObject.Bind();
}

void Mesh::UnBind() const {
	m_arrayObject.UnBind();
	m_bufferObject.UnBind();
	m_indexObject.UnBind();
}


BufferObject::BufferObject(const void* data, unsigned int size) : m_renderID(0) {
	GLCALL(glGenBuffers(1, &m_renderID));
	Bind();
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

BufferObject::~BufferObject() {
	GLCALL(glDeleteBuffers(1, &m_renderID));
}

void BufferObject::Bind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
}

void BufferObject::UnBind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


ArrayObject::ArrayObject() : m_renderID(0) {
	GLCALL(glGenVertexArrays(1, &m_renderID));
}

ArrayObject::~ArrayObject() {
	GLCALL(glDeleteVertexArrays(1, &m_renderID));
}

void ArrayObject::AddBuffer(const BufferObject& vb, const LayoutObject& layout) {
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * BufferElement::GetSizeOfType(element.type);
	}
}

void ArrayObject::Bind() const {
	GLCALL(glBindVertexArray(m_renderID));
}

void ArrayObject::UnBind() const {
	GLCALL(glBindVertexArray(0));
}


IndexObject::IndexObject(const unsigned int* data, unsigned int count) : m_renderID(0), m_count(0) {
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCALL(glGenBuffers(1, &m_renderID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
} 

IndexObject::~IndexObject() {
	GLCALL(glDeleteBuffers(1, &m_renderID));
}

void IndexObject::Bind() const {
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
}

void IndexObject::UnBind() const {
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

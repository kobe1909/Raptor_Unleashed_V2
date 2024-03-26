#include "mesh.h"

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

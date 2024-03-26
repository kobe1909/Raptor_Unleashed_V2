#pragma once

class BufferObject {
private:
	unsigned int m_renderID;

public:
	BufferObject(const void* data, unsigned int size);
	~BufferObject();

	void Bind() const;
	void UnBind() const;
};
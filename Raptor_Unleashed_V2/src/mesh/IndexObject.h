#pragma once

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
#pragma once
#include <vector>
#include <glad/glad.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;
};


struct VertexBufferLayout {
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

	VertexBufferLayout() :stride(0) {};

	template<typename T>
	void push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count) {
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * sizeof(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count) {
		stride += count * sizeof(GL_UNSIGNED_INT);
		elements.push_back({ GL_UNSIGNED_INT, count, false });
	}

	template<>
	void push<unsigned char>(unsigned int count) {
		elements.push_back({ GL_UNSIGNED_BYTE, count, false });
		stride += count * sizeof(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& getElements() const {
		return elements;
	}


	inline unsigned int getStride() const{
		return stride;
	}
};
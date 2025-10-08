#include "SimView.hpp"

namespace SimView
{
	IArray::IArray()
	{
		this->hasArray = false;
	}

	IArray::IArray(int elemCount, int* data)
	{
		GLuint id;
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemCount * sizeof(int), data, GL_DYNAMIC_DRAW);
		this->id = id;
		this->count = elemCount;
		this->hasArray = true;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IArray::IArray(IArray& other)
	{
		if (this->hasArray)
		{
			glDeleteBuffers(1, &id);
		}
		this->id = other.id;
		this->count = other.count;
		this->hasArray = other.hasArray;
		other.hasArray = false;
	}

	IArray& IArray::operator=(IArray&& other)
	{
		if (this != &other)
		{
			if (this->hasArray)
			{
				glDeleteBuffers(1, &id);
			}
			this->id = other.id;
			this->count = other.count;
			this->hasArray = other.hasArray;
			other.hasArray = false;
		}
		return *this;
	}

	IArray::~IArray()
	{
		if (hasArray)
		{
			glDeleteBuffers(1, &id);
		}
	}

	void IArray::Set(int index, int elemCount, int* data)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index * sizeof(int), elemCount * sizeof(int), data);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IArray::Destroy()
	{
		if (hasArray)
		{
			glDeleteBuffers(1, &id);
		}
		hasArray = false;
	}
}
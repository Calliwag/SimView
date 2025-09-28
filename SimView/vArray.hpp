#pragma once

#include "Includes.hpp"

namespace SimView
{
	class vArray
	{
	public:
		GLuint id;
		int count;
		bool hasArray;

		vArray()
		{
			this->hasArray = false;
		}
		vArray(GLuint id, int count)
		{
			this->id = id;
			this->count = count;
			this->hasArray = true;
		}
		vArray(vArray& other)
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
		~vArray()
		{
			if (hasArray)
			{
				glDeleteBuffers(1, &id);
			}
		}

		static vArray Init(int size, glm::vec2 points[]);
		void Set(int index, int size, glm::vec2 points[]);
	};
}
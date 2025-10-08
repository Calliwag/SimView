#include "SimView.hpp"

namespace SimView
{
    VArray::VArray()
    {
        this->hasArray = false;
    }

    VArray::VArray(int elemCount, int elemSize, void* data)
    {
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, elemCount * elemSize * sizeof(float), data, GL_DYNAMIC_DRAW);
        this->id = id;
        this->count = elemCount;
        this->elemSize = elemSize;
        this->hasArray = true;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VArray::VArray(VArray& other)
    {
        if (this->hasArray)
        {
            glDeleteBuffers(1, &id);
        }
        this->id = other.id;
        this->count = other.count;
        this->elemSize = other.elemSize;
        this->hasArray = other.hasArray;
        other.hasArray = false;
    }

    VArray& VArray::operator=(VArray&& other)
    {
        if (this != &other)
        {
            if (this->hasArray)
            {
                glDeleteBuffers(1, &id);
            }
            this->id = other.id;
            this->count = other.count;
            this->elemSize = other.elemSize;
            this->hasArray = other.hasArray;
            other.hasArray = false;
        }
        return *this;
    }

    VArray::~VArray()
    {
        if (hasArray)
        {
            glDeleteBuffers(1, &id);
        }
    }

    void VArray::Set(int index, int elemCount, void* data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferSubData(GL_ARRAY_BUFFER, index * elemSize * sizeof(float), elemCount * elemSize * sizeof(float), data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void VArray::Destroy()
    {
        if (hasArray)
        {
            glDeleteBuffers(1, &id);
        }
        hasArray = false;
    }
}
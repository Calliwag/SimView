#include "SimView.hpp"

namespace SimView
{
    vArray::vArray()
    {
        this->hasArray = false;
    }

    vArray::vArray(GLuint id, int count, int elemSize)
    {
        this->id = id;
        this->count = count;
        this->elemSize = elemSize;
        this->hasArray = true;
    }

    vArray::vArray(vArray& other)
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

    vArray::~vArray()
    {
        if (hasArray)
        {
            glDeleteBuffers(1, &id);
        }
    }

    vArray vArray::Init(int size, float values[])
    {
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), (float*)values, GL_STATIC_DRAW);
        return vArray(id, size, 1);
    }

    vArray vArray::Init(int size, glm::vec2 points[])
    {
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec2), (float*)points, GL_STATIC_DRAW);
        return vArray(id, size, 2);
    }

    vArray vArray::Init(int size, Color colors[])
    {
        FColor* fColors = new FColor[size];
        for (int i = 0; i < size; i++)
        {
            fColors[i] = FColor::Convert(colors[i]);
        }
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(FColor), (float*)fColors, GL_STATIC_DRAW);
        return vArray(id, size, 4);
    }

    vArray vArray::Init(int size, FColor colors[])
    {
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(FColor), (float*)colors, GL_STATIC_DRAW);
        return vArray(id, size, 4);
    }

    void vArray::Set(int index, int size, float values[])
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(float), size * sizeof(float), (float*)values);
    }

    void vArray::Set(int index, int size, glm::vec2 points[])
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::vec2), size * sizeof(glm::vec2), (float*)points);
    }
    void vArray::Set(int index, int size, Color colors[])
    {
        FColor* fColors = new FColor[size];
        for (int i = 0; i < size; i++)
        {
            fColors[i] = FColor::Convert(colors[i]);
        }
        Set(index, size, fColors);
        delete[] fColors;
    }
    void vArray::Set(int index, int size, FColor colors[])
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(FColor), size * sizeof(FColor), (float*)colors);
    }
}
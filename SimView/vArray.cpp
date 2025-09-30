#include "vArray.hpp"

namespace SimView
{
	vArray vArray::Init(int size, glm::vec2 points[])
    {
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec2), (float*)points, GL_STATIC_DRAW);
        return vArray(id, size);
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
        return vArray(id, size);
    }

    vArray vArray::Init(int size, FColor colors[])
    {
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(FColor), (float*)colors, GL_STATIC_DRAW);
        return vArray(id, size);
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
#include "vArray.hpp"

namespace SimView
{
	vArray vArray::Init(int size, glm::vec2 points[])
    {
        GLuint id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, 2 * size * sizeof(float), (float*)points, GL_STATIC_DRAW);
        return vArray(id, size);
    }

    void vArray::Set(int index, int size, glm::vec2 points[])
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferSubData(GL_ARRAY_BUFFER, 2 * index * sizeof(float), 2 * size * sizeof(float), (float*)points);
    }
}
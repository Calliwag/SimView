#include "SimView.hpp"

namespace SimView
{
    ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource, bool instanced, std::vector<std::string> uniforms, std::vector<std::string> attribs)
    {
        this->instanced = instanced;

        GLuint vertexShader;
        GLuint fragmentShader;
        int success;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            throw std::runtime_error("Renderer Error: Failed to compile vertex shader\n" + std::string(infoLog) + "\n");
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            throw std::runtime_error("Renderer Error: Failed to compile fragment shader\n" + std::string(infoLog) + "\n");
        }

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        glGetShaderiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            throw std::runtime_error("Renderer Error: Failed to compile shader program\n" + std::string(infoLog) + "\n");
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glUseProgram(id);
        
        for (std::string var : uniforms)
        {
            varLocs[var] = glGetUniformLocation(id, var.c_str());
        }
        for (std::string var : attribs)
        {
            varLocs[var] = glGetAttribLocation(id, var.c_str());
        }

        glUseProgram(0);

        instanceCount = 0;
    }

    void ShaderProgram::BindProgram()
    {
        glUseProgram(id);
    }

    void ShaderProgram::Draw(GLenum drawCall, GLint index, GLsizei count)
    {
        if (instanced)
        {
            glDrawArraysInstanced(drawCall, index, count, instanceCount);
        }
        else
        {
            glDrawArrays(drawCall, index, count);
        }
    }

    void ShaderProgram::BindArray(vArray& array, std::string name)
    {
        if (!varLocs.contains(name))
            throw std::runtime_error("Shader Error: Shader variable not found\n");
        GLint loc = varLocs[name];
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, array.elemSize, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
    }

    void ShaderProgram::BindColor(Color& color, std::string name)
    {
        if (!varLocs.contains(name))
            throw std::runtime_error("Shader Error: Shader variable not found\n");
        GLint loc = varLocs[name];
        glUniform4f(loc, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
    }

    void ShaderProgram::BindInstanceArray(vArray& array, std::string name)
    {
        if (!varLocs.contains(name))
            throw std::runtime_error("Shader Error: Shader variable not found\n");
        GLint loc = varLocs[name];
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, array.elemSize, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
        glVertexAttribDivisor(loc, 1);
        instanceCount = array.count;
    }

    void ShaderProgram::BindTexture(Texture& texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }

    void ShaderProgram::BindMat3x3(glm::mat3x3& matrix, std::string name)
    {
        if (!varLocs.contains(name))
            throw std::runtime_error("Shader Error: Shader variable not found\n");
        GLint loc = varLocs[name];
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void ShaderProgram::RenderTri(int index)
    {
        Draw(GL_TRIANGLES, index, 3);
    }

    void ShaderProgram::RenderQuad(int index)
    {
        Draw(GL_TRIANGLE_FAN, index, 4);
    }

    void ShaderProgram::RenderLine(int index)
    {
        Draw(GL_LINES, index, 2);
    }

    void ShaderProgram::RenderLines(int count, int index)
    {
        Draw(GL_LINE_STRIP, index, count + 1);
    }

    void ShaderProgram::RenderPolyline(int count, int index)
    {
        Draw(GL_LINE_LOOP, index, count);
    }

    void ShaderProgram::RenderPoints(int count, int index)
    {
        Draw(GL_POINTS, index, count);
    }
}
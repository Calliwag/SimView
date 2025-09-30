#include "Window.hpp"
#include <algorithm>

namespace SimView
{
    static void close_callback(GLFWwindow* window)
    {
        glfwSetWindowShouldClose(window, 1);
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    const char* vertexShaderFlatSource = /* vertex shader:*/ R"(

        #version 460 core
        in vec2 position;
        uniform mat3 transform;

        void main()
        {
            gl_Position = vec4(transform * vec3(position, 1.0f),1.0);
        };

)";

    const char* fragmentShaderFlatSource = /* fragment shader:*/ R"(

        #version 460 core
        uniform vec4 renderColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = renderColor;
        }

)";

    const char* vertexShaderTextureSource = /* vertex shader:*/ R"(

        #version 460 core
        in vec2 position;
        uniform mat3 transform;
        in vec2 inputTexCoord;
        out vec2 texCoord;
        void main()
        {
            gl_Position = vec4(transform * vec3(position, 1.0f),1.0);
            texCoord = inputTexCoord;
        };

)";

    const char* fragmentShaderTextureSource = /* fragment shader:*/ R"(

        #version 460 core
        in vec2 texCoord;
        uniform vec4 renderColor;
        uniform sampler2D inputTexture;
        out vec4 FragColor;
        void main()
        {
            FragColor = texture(inputTexture, texCoord) * renderColor;
        }

)";

    const char* vertexShaderInstanceTextureSource = /* vertex shader:*/ R"(

        #version 460 core
        in vec2 position;
        uniform mat3 transform;
        in vec2 inputTexCoord;
        in vec2 offset;
        out vec2 texCoord;
        void main()
        {
            gl_Position = vec4(transform * vec3(offset + position, 1.0f), 1.0f);
            texCoord = inputTexCoord;
        };

)";

    const char* vertexShaderInstanceTextureColorSource = /* vertex shader:*/ R"(

        #version 460 core
        in vec2 position;
        in vec2 inputTexCoord;
        in vec2 offset;
        in vec4 inColor;
        uniform mat3 transform;
        out vec2 texCoord;
        out vec4 color;
        void main()
        {
            gl_Position = vec4(transform * vec3(offset + position, 1.0f), 1.0f);
            texCoord = inputTexCoord;
            color = inColor;
        };

)";

    const char* fragmentShaderInstanceTextureColorSource = /* fragment shader:*/ R"(

        #version 460 core
        in vec2 texCoord;
        in vec4 color;
        uniform sampler2D inputTexture;
        out vec4 FragColor;
        void main()
        {
            FragColor = texture(inputTexture, texCoord) * color;
        }

)";

    ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource, bool instanced)
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

        renderColorLoc = glGetUniformLocation(id, "renderColor");
        transMatLoc = glGetUniformLocation(id, "transform");
        vertexPosLoc = glGetAttribLocation(id, "position");
        vertexUVLoc = glGetAttribLocation(id, "inputTexCoord");
        instanceArrLoc = glGetAttribLocation(id, "offset");
        vertexColorLoc = glGetAttribLocation(id, "inColor");

        glUseProgram(0);
    }

    void ShaderProgram::Use()
    {
        glUseProgram(id);
    }

    void ShaderProgram::Draw(GLenum drawCall, GLint index, GLsizei count)
    {
        if (instanced)
        {
            glDrawArraysInstanced(drawCall, index, count, instanceArray->count);
        }
        else
        {
            glDrawArrays(drawCall, index, count);
        }
    }

    void ShaderProgram::BindPosArray(vArray& array)
    {
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        glEnableVertexAttribArray(vertexPosLoc);
        glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
        posArray = &array;
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
    }

    void ShaderProgram::UnbindPosArray()
    {
        glDisableVertexAttribArray(vertexPosLoc);
        posArray = nullptr;
    }

    void ShaderProgram::BindUVArray(vArray& array)
    {
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        glEnableVertexAttribArray(vertexUVLoc);
        glVertexAttribPointer(vertexUVLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
        uvArray = &array;
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
    }

    void ShaderProgram::UnbindUVArray()
    {
        glDisableVertexAttribArray(vertexUVLoc);
        uvArray = nullptr;
    }

    void ShaderProgram::BindInstanceArray(vArray& array)
    {
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        glEnableVertexAttribArray(instanceArrLoc);
        glVertexAttribPointer(instanceArrLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
        instanceArray = &array;
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
        glVertexAttribDivisor(instanceArrLoc, 1);
    }

    void ShaderProgram::UnbindInstanceArray()
    {
        glDisableVertexAttribArray(instanceArrLoc);
        instanceArray = nullptr;
    }

    void ShaderProgram::BindTexture(Texture& tex)
    {
        glBindTexture(GL_TEXTURE_2D, tex.id);
        texture = &tex;
    }

    void ShaderProgram::UnbindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        texture = nullptr;
    }

    void ShaderProgram::BindColorArray(vArray& array)
    {
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        glEnableVertexAttribArray(vertexColorLoc);
        glVertexAttribPointer(vertexColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
        colorArray = &array;
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
        if (instanced)
            glVertexAttribDivisor(vertexColorLoc, 1);
    }

    void ShaderProgram::UnbindColorArray()
    {
        glDisableVertexAttribArray(vertexColorLoc);
        colorArray = nullptr;
    }

    void ShaderProgram::SetRenderColor(Color color)
    {
        glUniform4f(renderColorLoc, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
    }

    void ShaderProgram::SetTransformMatrix(glm::mat3x3& matrix)
    {
        glUniformMatrix3fv(transMatLoc, 1, GL_FALSE, glm::value_ptr(matrix));
        currentMatrix = matrix;
    }

    Window::Window(int width, int height, std::string title)
    {
        int err;

        windowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwSetWindowCloseCallback(windowPtr, close_callback);
        glfwSetFramebufferSizeCallback(windowPtr, framebuffer_size_callback);

        if (!windowPtr)
        {
            glfwTerminate();
            throw std::runtime_error("Window Error: Failed to initialize window\n");;
        }

        glfwMakeContextCurrent(windowPtr);
        int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (version == 0) {
            throw std::runtime_error("Window Error: Failed to initialize OpenGL context\n");
        }
        glfwMakeContextCurrent(NULL);

        BeginContext();

        glfwSwapInterval(0);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glEnable(GL_BLEND);

        BeginFrame();
        EndFrame();

        EndContext();
    }

    Window Window::Create(int width, int height, std::string title)
    {
        return Window(width, height, title);
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(windowPtr);
    }

    void Window::BeginContext()
    {
        glfwMakeContextCurrent(windowPtr);
    }

    void Window::EndContext()
    {
        glfwMakeContextCurrent(NULL);
    }

    void Window::BeginFrame()
    {
        frameTime = frameStartTime;
        frameStartTime = glfwGetTime();
        frameTime = frameStartTime - frameTime;
        glfwGetFramebufferSize(windowPtr, &width, &height);
        glViewport(0, 0, width, height);
        viewMatrix = GetViewMatrix();
    }

    void Window::EndFrame()
    {
        glfwSwapBuffers(windowPtr);
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::Destroy()
    {
        glfwDestroyWindow(windowPtr);
    }

    void Window::SetLineWidth(int width)
    {
        glLineWidth(width);
    }

    void Window::SetPointSize(int size)
    {
        glPointSize(size);
    }

    glm::mat3x3 Window::GetViewMatrix()
    {
        glm::mat3x3 matrix;
        matrix[0][0] = 2.f / width;
        matrix[1][0] = 0;
        matrix[2][0] = -1;
        matrix[0][1] = 0;
        matrix[1][1] = 2.f / height;
        matrix[2][1] = -1;
        matrix[0][2] = 0;
        matrix[1][2] = 0;
        matrix[2][2] = 1;
        return matrix;
    }

    void Window::SetShader(ShaderProgram& shaderProgram)
    {
        currentShader = &shaderProgram;
        currentShader->Use();
    }

    ShaderProgram Window::GetFlatShader()
    {
        return ShaderProgram(vertexShaderFlatSource,fragmentShaderFlatSource, false);
    }

    ShaderProgram Window::GetTexShader()
    {
        return ShaderProgram(vertexShaderTextureSource,fragmentShaderTextureSource, false);
    }

    ShaderProgram Window::GetInstTexShader()
    {
        return ShaderProgram(vertexShaderInstanceTextureSource, fragmentShaderTextureSource, true);
    }

    ShaderProgram Window::GetInstTexColorShader()
    {
        return ShaderProgram(vertexShaderInstanceTextureColorSource, fragmentShaderInstanceTextureColorSource, true);
    }

    void Window::SetBlendMode(BlendMode mode)
    {
        switch (mode)
        {
        case(BlendMode::Default):
            glDisable(GL_BLEND);
            break;
        case(BlendMode::Alpha):
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case(BlendMode::Add):
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            glBlendEquation(GL_FUNC_ADD);
            break;
        }
    }

    void Window::FillScreen(Color color)
    {
        glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    double Window::GetFPS()
    {
        return 1.0 / frameTime;
    }

    void Window::RenderTri(int index)
    {
        currentShader->Draw(GL_TRIANGLES, index, 3);
    }

    void Window::RenderQuad(int index)
    {
        currentShader->Draw(GL_TRIANGLE_FAN, index, 4);
    }

    void Window::RenderLine(int index)
    {
        currentShader->Draw(GL_LINES, index, 2);
    }

    void Window::RenderLines(int index, int count)
    {
        if (count == 0)
            count = currentShader->posArray->count - 1 - index;
        else
            count += 1;

        currentShader->Draw(GL_LINE_STRIP, index, count);
    }

    void Window::RenderPolyline(int index, int count)
    {
        if (count == 0)
            count = currentShader->posArray->count - index;
        else
            count += 1;

        currentShader->Draw(GL_LINE_LOOP, index, count - 1);
    }

    void Window::RenderPoints(int index, int count)
    {
        if (count == 0)
            count = currentShader->posArray->count - index;

        currentShader->Draw(GL_POINTS, index, count);
    }
}
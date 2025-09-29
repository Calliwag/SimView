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

    const char* vertexShaderSource =
        "#version 460 core\n"
        "in vec2 position;\n"
        "uniform mat3 transform;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(transform * vec3(position, 1.0f),1.0);\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 460 core\n"
        "uniform vec4 renderColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = renderColor;\n"
        "}\0";

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

        GLuint vertexShader;
        GLuint fragmentShader;
        int success;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            throw std::runtime_error("Renderer Error: Failed to compile vertex shader\n" + std::string(infoLog) + "\n");
        }
        err = glGetError();

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            throw std::runtime_error("Renderer Error: Failed to compile fragment shader\n" + std::string(infoLog) + "\n");
        }
        err = glGetError();

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
            throw std::runtime_error("Renderer Error: Failed to compile shader program\n" + std::string(infoLog) + "\n");
        }
        err = glGetError();

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glUseProgram(shaderProgram);

        renderColorLoc = glGetUniformLocation(shaderProgram, "renderColor");
        transMatLoc = glGetUniformLocation(shaderProgram, "transform");
        vertexPosLoc = glGetAttribLocation(shaderProgram, "position");

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glEnable(GL_BLEND);

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
        frameStartTime = glfwGetTime();
        glfwGetFramebufferSize(windowPtr, &width, &height);
        glViewport(0, 0, width, height);
        viewMatrix = GetViewMatrix();
        glUseProgram(shaderProgram);
    }

    void Window::EndFrame()
    {
        glfwSwapBuffers(windowPtr);
        frameEndTime = glfwGetTime();
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::Destroy()
    {
        glfwDestroyWindow(windowPtr);
    }

    void Window::SetRenderColor(Color color)
    {
        glUniform4f(renderColorLoc, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
    }

    void Window::SetLineWidth(int width)
    {
        glLineWidth(width);
    }

    void Window::SetPointSize(int size)
    {
        glPointSize(size);
    }

    void Window::SetTransformMatrix(glm::mat3x3& matrix)
    {
        glUniformMatrix3fv(transMatLoc, 1, GL_FALSE, glm::value_ptr(matrix));
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

    void Window::BindVArray(vArray& array)
    {
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        glEnableVertexAttribArray(vertexPosLoc);
        glVertexAttribPointer(vertexPosLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    void Window::SetBlendMode(BlendMode mode)
    {
        switch (mode)
        {
        case(BlendMode::Default):
            glDisable(GL_BLEND);
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
        return 1.0 / (frameEndTime - frameStartTime);
    }

    void Window::RenderTri(vArray& array, int index)
    {
        BindVArray(array);
        SetTransformMatrix(viewMatrix);

        glDrawArrays(GL_TRIANGLES, index, 3);
    }

    void Window::RenderLine(vArray& array, int index)
    {
        BindVArray(array);
        SetTransformMatrix(viewMatrix);

        glDrawArrays(GL_LINES, index, 2);
    }

    void Window::RenderLines(vArray& array, int index, int count)
    {
        if (count == 0)
            count = array.count - 1 - index;
        else
            count += 1;

        BindVArray(array);
        SetTransformMatrix(viewMatrix);

        glDrawArrays(GL_LINE_STRIP, index, count);
    }
    void Window::RenderPolyline(vArray& array, int index, int count)
    {
        if (count == 0)
            count = array.count - index;
        else
            count += 1;

        BindVArray(array);
        SetTransformMatrix(viewMatrix);

        glDrawArrays(GL_LINE_LOOP, index, count - 1);
    }
    void Window::RenderPoints(vArray& array, int index, int count)
    {
        if (count == 0)
            count = array.count - index;

        BindVArray(array);
        SetTransformMatrix(viewMatrix);

        glDrawArrays(GL_POINTS, index, count);
    }
}
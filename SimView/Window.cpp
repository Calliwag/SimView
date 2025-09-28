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
        "#version 330 core\n"
        "in vec2 position;\n"
        "uniform vec2 windowSize;"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(2 * position.x / windowSize.x - 1.0, 2 * position.y / windowSize.y - 1.0, 0.0, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "uniform vec4 OutputColor;"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = OutputColor;\n"
        "}\0";

    Window::Window(int width, int height, std::string title)
    {
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

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            throw std::runtime_error("Renderer Error: Failed to initialize shader program\n");
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        renderColorLoc = glGetUniformLocation(shaderProgram, "OutputColor");
        windowSizeLoc = glGetUniformLocation(shaderProgram, "windowSize");

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
        glUniform2f(windowSizeLoc, width, height);
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

    void Window::RenderTriFull2D(vArray& array)
    {
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glUseProgram(shaderProgram);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void Window::RenderLine2D(vArray& array)
    {
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glUseProgram(shaderProgram);

        glDrawArrays(GL_LINES, 0, 2);
    }

    void Window::RenderLines2D(vArray& array)
    {
        glBindBuffer(GL_ARRAY_BUFFER, array.id);
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glUseProgram(shaderProgram);

        for (int i = 0; i < array.count - 1; i++)
        {
            glDrawArrays(GL_LINES, i, 2);
        }
    }
}
#include "SimView.hpp"
#include <algorithm>

namespace SimView
{
    Window* callbackWindow = nullptr;

    static void close_callback(GLFWwindow* window)
    {
        glfwSetWindowShouldClose(window, 1);
    }

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        if (!callbackWindow)
            return;
        glViewport(0, 0, width, height);
        callbackWindow->width = width;
        callbackWindow->height = height;
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (!callbackWindow)
            return;
        if (action == GLFW_PRESS)
        {
            callbackWindow->keyDown[scancode] = true;
        }
        if (action == GLFW_RELEASE)
        {
            callbackWindow->keyDown[scancode] = false;
        }
    }



    Window::Window(int width, int height, std::string title)
    {
        int err;

        this->width = width;
        this->height = height;

        windowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwSetWindowCloseCallback(windowPtr, close_callback);
        glfwSetFramebufferSizeCallback(windowPtr, framebuffer_size_callback);
        glfwSetKeyCallback(windowPtr, key_callback);

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

        glfwSwapInterval(0);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glEnable(GL_BLEND);

        BeginFrame();
        PollEvents();
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
        callbackWindow = this;
    }

    void Window::EndContext()
    {
        glfwMakeContextCurrent(NULL);
        callbackWindow = nullptr;
    }

    void Window::BeginFrame()
    {
        frameTime = frameStartTime;
        frameStartTime = glfwGetTime();
        frameTime = frameStartTime - frameTime;
        viewMatrix = GetViewMatrix();
    }

    void Window::EndFrame()
    {
        glfwSwapBuffers(windowPtr);
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
        lastKeyDown = keyDown;
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

    bool Window::IsKeyPressed(int glfwKey)
    {
        int key = glfwGetKeyScancode(glfwKey);
        return keyDown[key] && !lastKeyDown[key];
    }

    bool Window::IsKeyDown(int glfwKey)
    {
        int key = glfwGetKeyScancode(glfwKey);
        return keyDown[key];
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
}
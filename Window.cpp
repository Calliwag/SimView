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

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (!callbackWindow)
            return;
        glm::vec2 newPos = { xpos,ypos };
        callbackWindow->mouseDelta = newPos - callbackWindow->mousePos;
        callbackWindow->mousePos = newPos;
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (!callbackWindow)
            return;
        if (action == GLFW_PRESS)
        {
            callbackWindow->mButtonDown[button] = true;
        }
        if (action == GLFW_RELEASE)
        {
            callbackWindow->mButtonDown[button] = false;
        }
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if (!callbackWindow)
            return;
        callbackWindow->scrollDelta = { xoffset,yoffset };
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
        glfwSetCursorPosCallback(windowPtr, cursor_position_callback);
        glfwSetMouseButtonCallback(windowPtr, mouse_button_callback);
        glfwSetScrollCallback(windowPtr, scroll_callback);

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
    }

    void Window::EndFrame()
    {
        glfwSwapBuffers(windowPtr);
    }

    void Window::PollEvents()
    {
        lastKeyDown = keyDown;
        lastMButtonDown = mButtonDown;
        mouseDelta = { 0,0 };
        scrollDelta = { 0,0 };
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

    bool Window::IsMouseButtonPressed(int glfwMouseButton)
    {
        return mButtonDown[glfwMouseButton] && !lastMButtonDown[glfwMouseButton];
    }

    bool Window::IsMouseButtonDown(int glfwMouseButton)
    {
        return mButtonDown[glfwMouseButton];
    }

    glm::vec2 Window::GetScrollDelta()
    {
        return scrollDelta;
    }

    void Window::LockMouse()
    {
        glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::UnlockMouse()
    {
        glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
        case(BlendMode::PreMultAlpha):
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
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
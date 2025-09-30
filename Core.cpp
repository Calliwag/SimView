#include "SimView.hpp"
#include <print>

namespace SimView
{
    static void error_callback(int error, const char* description)
    {
        std::println(stderr, "GLFW Error: {}\n", description);
    }

    void Core::Init()
    {
        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
            throw std::runtime_error("Core Error: Failed to initialize\n");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void Core::DeInit()
    {
        glfwTerminate();
    }
}
/**
 * @file gl-sandbox.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "glpch.h"

int main(int argc, char** argv) {  //NOLINT
    GLFWwindow* window = nullptr;

    /* (GLFW) Initialize GLFW*/
    if (glfwInit() == GLFW_FALSE) {
        return -1;
    }

    /* (GLFW) Create a window and its context */
#if defined(PLATFORM_WINDOWS)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPACT_PROFILE);
#elif defined(PLATFORM_UNIX)
#elif defined(PLATFORM_APPLE)
    /* To use OpenGL 4.1 */
    // glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE);  // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* (GLAD) Initialize GLAD */
    gladLoadGL();
    // int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // if (status == 0) {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }
    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /* Render here */
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        /* set background color */
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // dark grey

        /* clear the framebuffer with seted background color */
        glClear(GL_COLOR_BUFFER_BIT);

        /* (GLFW) Swap front and back buffers */
        glfwSwapBuffers(window);

        /* (GLFW) Poll for and process events */
        glfwPollEvents();
    }

    /* Delete window before ending the program */
    glfwDestroyWindow(window);
    /* Terminate GLFW before ending the program */
    glfwTerminate();
    return 0;
}
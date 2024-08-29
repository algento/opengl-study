/**
 * @file test_gui_event.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-26
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

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

#include <iostream>

#include "gl-core/event/gui_event.h"
#include "gl-core/event/key_code.h"
#include "gl-core/event/key_event.h"

#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif

//NOLINTBEGIN
double delta_time = 0.0F;
double last_time  = 0.0F;
//NOLINTEND

struct WindowData {
    std::string title;
    std::function<bool(glcore::GuiEvent&)> event_callback;
};

bool OnEventKeyPressed(glcore::GuiEvent& event) {
    std::cout << event.ToString() << "\n";
    return event.handled();
}

bool OnEvent(glcore::GuiEvent& event) {
    glcore::GuiEventHandler handler(event);
    handler.Process<glcore::KeyPressedEvent>(OnEventKeyPressed);
    return event.handled();
}

int32_t main(int32_t argc, char** argv) {  //NOLINT
    GLFWwindow* window = nullptr;

    /* (GLFW) GLFW 초기화*/
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "Failed to initialize GLFW" << '\n';
        glfwTerminate();
        return -1;
    }

    /* (GLFW) 윈도우 힌트 지정 */
    glfwWindowHint(GLFW_SAMPLES, 4);  // 4x Antialiasing

#if defined(PLATFORM_WINDOWS)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPACT_PROFILE);
#elif defined(PLATFORM_UNIX)
#elif defined(PLATFORM_APPLE)
    /* To use OpenGL 4.1 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE);  // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    /* (GLFW) 윈도우 생성 */
    window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* (GLAD) GLAD 초기화 */
#if GLCORE_USE_GLEW
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW initialisation failed!" << "\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
#else
    gladLoadGL();
#endif

    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    glEnable(GL_DEPTH_TEST);  // Depth 설정, Depth에 따른 UI 그리려면 필요함.

    // Event Callback 설정
    WindowData window_data{};
    glfwSetWindowUserPointer(window, &window_data);
    window_data.event_callback = OnEvent;
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode,
                                  int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS: {
                glcore::KeyPressedEvent event(static_cast<glcore::KeyCode>(key),
                                              false);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE: {
                glcore::KeyReleasedEvent event(
                    static_cast<glcore::KeyCode>(key));
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT: {
                glcore::KeyPressedEvent event(static_cast<glcore::KeyCode>(key),
                                              true);
                data.event_callback(event);
                break;
            }

            default:
                break;
        }
    });

    /* 랜더링 루프 --------------------------------------------------------------*/
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        double now = glfwGetTime();
        delta_time = now - last_time;
        last_time  = now;
        /* 배경색 설정 */
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // dark grey

        /* 프레임 버퍼를 배경색으로 최기화 */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);

        /* (GLFW) 프로세스 이벤트 폴링 */
        glfwPollEvents();
    }

    /* (GLFW) 윈도우 파괴 및 종료*/
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
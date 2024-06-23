/**
 * @file user_input.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-22
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */
#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "gl-core/event/key_code.h"
#include "gl-core/event/mouse_code.h"

namespace glcore {
class UserInput {
 public:
    static bool IsKeyPressed(GLFWwindow* window, KeyCode keycode) {
        auto state = glfwGetKey(window, static_cast<int>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    static bool IsMouseButtonPressed(GLFWwindow* window, MouseCode button) {
        auto state = glfwGetMouseButton(window, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    static float CheckMouseScrollYOffset(GLFWwindow* window) {
        auto state = glfwGetWindowUserPointer(window);
        
    }

    static glm::vec2 GetMousePosition(GLFWwindow* window) {
        double x = 0.0F;
        double y = 0.0F;
        glfwGetCursorPos(window, &x, &y);

        return {(float)x, (float)y};
    }

    static float GetMouseX(GLFWwindow* window) {
        return GetMousePosition(window).x;
    }
    static float GetMouseY(GLFWwindow* window) {
        return GetMousePosition(window).y;
    }
};
}  // namespace glcore
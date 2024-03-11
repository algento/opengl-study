/**
 * @file window.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-15
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once

#include <GLFW/glfw3.h>

#include <functional>

namespace glcore {
using EventCallback = std::function<void(int)>;

struct WindowProps {
    std::string title;
    uint32_t width;
    uint32_t height;
    bool vsync{false};
    EventCallback event_callback;

    explicit WindowProps(std::string title = "gl-core", uint32_t width = 1280,
                         uint32_t height = 720)
        : title(std::move(title)), width(width), height(height) {}
};

class Window {
 public:
    Window() = default;
    Window(std::string title, uint32_t width, uint32_t height);
    virtual ~Window()                 = default;
    Window(const Window &)            = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&)                 = default;
    Window &operator=(Window &&)      = default;

    void OnUpdate();
    void SetVSync(bool enabled);
    void SetEventCallback();

    [[nodiscard]] uint32_t GetWidth() const { return props_.width; }
    [[nodiscard]] uint32_t GetHeight() const { return props_.width; }
    [[nodiscard]] GLFWwindow *GetNativeWindow() const { return window_; }
    [[nodiscard]] bool IsVsync() const { return props_.vsync; }

 private:
    void Init();
    void Shutdown();

    GLFWwindow *window_{nullptr};
    WindowProps props_;
};
}  // namespace glcore
/**
 * @file key_event.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-28
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once

#include <sstream>

#include "gl-core/event/gui_event.h"
#include "gl-core/event/key_code.h"

namespace glcore {

class KeyPressedEvent : public GuiEvent {
 public:
    KeyPressedEvent(const KeyCode key_code, bool is_repeated)
        : GuiEvent(GuiEventType::kKeyPressed),
          code_(static_cast<uint32_t>(key_code)),
          is_repeated_(is_repeated) {}

    GUI_EVENT_CLASS_SETTING(kKeyPressed);

    [[nodiscard]] uint32_t code() const { return code_; }
    [[nodiscard]] bool is_repeated() const { return is_repeated_; }

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << code_ << ", is_repeated: " << is_repeated_;
        return ss.str();
    }

 private:
    uint32_t code_;
    bool is_repeated_;
};

class KeyReleasedEvent : public GuiEvent {
 public:
    explicit KeyReleasedEvent(const KeyCode key_code)
        : GuiEvent(GuiEventType::kKeyReleased),
          code_(static_cast<uint32_t>(key_code)) {}

    GUI_EVENT_CLASS_SETTING(kKeyReleased);

    [[nodiscard]] uint16_t code() const { return code_; }

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << code_;
        return ss.str();
    }

 private:
    uint32_t code_;
};

class KeyTypedEvent : public GuiEvent {
 public:
    explicit KeyTypedEvent(const KeyCode key_code)
        : GuiEvent(GuiEventType::kKeyPressed),
          code_(static_cast<uint32_t>(key_code)) {}

    GUI_EVENT_CLASS_SETTING(kKeyTyped);

    [[nodiscard]] uint16_t code() const { return code_; }

    [[nodiscard]] std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << code_;
        return ss.str();
    }

 private:
    uint32_t code_;
};
}  // namespace glcore
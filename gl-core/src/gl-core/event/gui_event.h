#include <utility>

/**
 * @file gui_event.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-12
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */

#pragma once
#include "gl-core/event/event_dispatcher.h"

namespace glcore {
#define BIT(x)       (1 << x)  // NOLINT
#define TO_STRING(x) #x        // NOLINT

enum class EventType : uint16_t {
    kNone = 0,
    kWindowClose,
    kWindowResize,
    kWindowFocus,
    kWindowLostFocus,
    kWindowMoved,
    kApplicationTick,
    kApplicationUpdate,
    kApplicationRender,
    kKeyPressed,
    kKeyReleased,
    kKeyTyped,
    kMouseButtonPressed,
    kMouseButtonReleased,
    kMouseMoved,
    kMouseScrolled,
    kAll
};

enum EventCategory : uint32_t {
    kNone                  = BIT(17),
    kEventCategoryInput    = BIT(18),  // NOLINT
    kEventCategoryKeyboard = BIT(19),  // NOLINT
    kEventCategoryMouse    = BIT(20),  // NOLINT
};

class GuiEvent {
 public:
    GuiEvent(std::string name, EventType type, EventCategory category)
        : id_((uint32_t)type + (uint32_t)category),
          name_(std::move(name)),
          type_(type),
          category_(category) {}

    [[nodiscard]] uint32_t id() const { return id_; }
    [[nodiscard]] EventType type() const { return type_; }
    [[nodiscard]] EventCategory category() const { return category_; }
    [[nodiscard]] const std::string& name() const { return name_; }
    [[nodiscard]] bool handled() const { return handled_; }

    void SetHandled(bool handled) { handled_ = handled; }
    bool IsInCategory(EventCategory category) {
        return (category & category_) != 0;
    }

 private:
    uint32_t id_{0};
    bool handled_{false};
    std::string name_;
    EventType type_;
    EventCategory category_;
};

using GuiEventDispatcher = EventDispatcher<GuiEvent, bool, uint32_t>;

}  // namespace glcore
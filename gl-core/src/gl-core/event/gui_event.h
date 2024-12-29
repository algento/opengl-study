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
#include <functional>
namespace glcore {
#define BIT(x)       (1 << x)  // NOLINT
#define TO_STRING(x) #x        // NOLINT

#define GUI_EVENT_CLASS_SETTING(TYPE) \
    using GuiEvent::handled;          \
    using GuiEvent::SetHandled;       \
    using GuiEvent::type;             \
    static GuiEventType GetStaticType() { return GuiEventType::TYPE; }

enum class GuiEventType : uint32_t {
    kNone = 0,
    kWindowClose,
    kWindowResize,
    kWindowFocus,
    kWindowLostFocus,
    kWindowMoved,
    kKeyPressed,
    kKeyReleased,
    kKeyTyped,
    kMouseButtonPressed,
    kMouseButtonReleased,
    kMouseMoved,
    kMouseScrolled,
    kAll
};

class GuiEvent {
 public:
    explicit GuiEvent(GuiEventType type) : type_(type) {}
    GuiEvent(const GuiEvent &)            = default;
    GuiEvent &operator=(const GuiEvent &) = default;
    GuiEvent(GuiEvent &&)                 = default;
    GuiEvent &operator=(GuiEvent &&)      = default;
    virtual ~GuiEvent()                   = default;

    [[nodiscard]] GuiEventType type() const { return type_; }
    [[nodiscard]] bool handled() const { return handled_; }
    [[nodiscard]] virtual std::string ToString() const = 0;

    void SetHandled(bool handled) { handled_ = handled; }

 protected:
    bool handled_{false};
    GuiEventType type_;
};

class GuiEventHandler {
 public:
    explicit GuiEventHandler(GuiEvent &event) : event_(event) {}

    // F는 일반적으로 입력하지 않고 컴파일러가 추론하게 한다.
    // template <typename T, typename F>
    template <typename T>
    bool Process(const std::function<bool(T &)> &fn) {
        bool check_event_type = event_.type() == T::GetStaticType();  //NOLINT
        if (check_event_type) {
            // event_.SetHandled(event_.handled() | fn(*((T *)&event_)));
            event_.SetHandled(event_.handled() | fn(static_cast<T &>(event_)));
        }
        return check_event_type;
    }

 private:
    // TODO: change to safe type
    GuiEvent &event_;  //NOLINT
};

inline std::ostream &operator<<(std::ostream &os, const GuiEvent &e) {
    return os << e.ToString();
}

}  // namespace glcore
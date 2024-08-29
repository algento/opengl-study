/**
 * @file event_dispatcher.h
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
#include <map>
#include <mutex>

namespace glcore {
/**
 * @brief 
 * 
 * @tparam EventType : struct that has id, name, is_handled
 * @tparam ReturnType 
 * @tparam Args 
 */
template <typename EventType, typename ReturnType, typename... Args>
class EventDispatcher {
 public:
    using event_t    = EventType;
    using listener_t = std::function<ReturnType(Args...)>;

    EventDispatcher()                                  = default;
    virtual ~EventDispatcher()                         = default;
    EventDispatcher(const EventDispatcher&)            = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;
    EventDispatcher(EventDispatcher&&)                 = delete;
    EventDispatcher& operator=(EventDispatcher&&)      = delete;

    void Dispatch(event_t event, Args&&... args) {
        if (listeners_.find(event.id) != listeners_.end()) {
            listeners_[event.id](std::forward<Args>(args)...);
        } else {
            std::cout << "EventDispatcher::Dispatch(): no listener for "
                      << event.name << std::endl;
        }
    }

    void AddListener(event_t event, listener_t listener) {
        listeners_[event.id] = listener;
    }

    void RemoveListener(event_t event) { listeners_.erase(event.id); }

 private:
    std::map<uint32_t, listener_t> listeners_;
    // std::mutex mutex_;
};

}  // namespace glcore
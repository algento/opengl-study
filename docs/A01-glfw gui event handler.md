# GLFW gui Event Handler

- <https://github.com/maxomous/Event-Dispatcher/blob/main/main.cpp>
- <https://codereview.stackexchange.com/questions/273162/event-dispatcher-for-glfw-glad>
- <https://gofo-coding.tistory.com/entry/GLFW-Input-handling>
- <https://www.gamedev.net/forums/topic/702413-is-my-user-event-handling-ok-glfwc/5406542/>
- <https://www.reddit.com/r/opengl/comments/16x9m8e/glfw_input_handler_class_code_review/>

```c++
#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
```

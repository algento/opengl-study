/**
 * @file mouse_code.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-12
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once
namespace glcore {
enum class MouseCode : uint32_t {
    // From glfw3.h
    Button0 = 0,
    Button1 = 1,
    Button2 = 2,
    Button3 = 3,
    Button4 = 4,
    Button5 = 5,
    Button6 = 6,
    Button7 = 7,

    ButtonLast   = Button7,
    ButtonLeft   = Button0,
    ButtonRight  = Button1,
    ButtonMiddle = Button2
};
}  // namespace glcore

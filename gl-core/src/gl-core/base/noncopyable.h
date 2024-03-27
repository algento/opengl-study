/**
 * @file noncopyable.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-22
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once

namespace glcore {
//NOLINTBEGIN
#define NONCOPYABLE(class_name)                         \
    class_name(const class_name &)            = delete; \
    class_name &operator=(const class_name &) = delete; \
    class_name(class_name &&)                 = delete; \
    class_name &operator=(class_name &&)      = delete

#define MOVABLE_NONCOPYABLE(class_name)                  \
    class_name(const class_name &)            = delete;  \
    class_name &operator=(const class_name &) = delete;  \
    class_name(class_name &&)                 = default; \
    class_name &operator=(class_name &&)      = default
//NOLINTEND

/// @brief Inherit (privately) to suppress copy/move construction and assignment.
class NonCopyable {
 public:
    NonCopyable()          = default;
    virtual ~NonCopyable() = default;

    NonCopyable(const NonCopyable &)            = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;

    NonCopyable(NonCopyable &&)            = delete;
    NonCopyable &operator=(NonCopyable &&) = delete;
};

/// @brief Inherit (privately) to suppress copy construction and assignment.
class MovableNonCopyable {
 public:
    MovableNonCopyable()          = default;
    virtual ~MovableNonCopyable() = default;

    MovableNonCopyable(const MovableNonCopyable &)            = delete;
    MovableNonCopyable &operator=(const MovableNonCopyable &) = delete;

    MovableNonCopyable(MovableNonCopyable &&)            = default;
    MovableNonCopyable &operator=(MovableNonCopyable &&) = default;
};

}  // namespace glcore
/**
 * @file singleton.h
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
template <typename T>
class Singleton {
 public:
    static T &GetInstance() {
        static T instance;
        return instance;
    }

    Singleton(const Singleton &)            = delete;
    Singleton &operator=(const Singleton &) = delete;

    Singleton(Singleton &&)            = delete;
    Singleton &operator=(Singleton &&) = delete;

 protected:
    Singleton()          = default;
    virtual ~Singleton() = default;

 private:
    // Singleton(Singleton const &)            = default;
    // Singleton &operator=(Singleton const &) = default;
};
}  // namespace glcore
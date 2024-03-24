/**
 * @file exception.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-24
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once
#include <string>

namespace glcore::internal {
inline std::string ExceptionMessage(const std::string_view& msg,
                                    const char* file_name,
                                    unsigned int line_number,
                                    const char* function_name) {
    std::string s;
    s += file_name;
    s += "@";
    s += std::to_string(line_number);
    s += ": [";
    s += function_name;
    s += "] ";
    s += msg;
    s += "\n";
    return s;
}

inline std::string ShortFileName(const std::string& path) {
    return path.substr(path.find_last_of("src") + 1);
}

}  // namespace glcore::internal

#define GLCORE_FILENAME_ \
    glcore::internal::ShortFileName(std::string(__FILE__)).c_str()

#define GLCORE_ASSERTMSG_(predicate, message)                                 \
    if (!(predicate)) {                                                       \
        throw ::std::logic_error(                                             \
            glcore::internal::ExceptionMessage(message, GLCORE_FILENAME_,     \
                                               __LINE__, __PRETTY_FUNCTION__) \
                .c_str());                                                    \
    }

#define GLCORE_ASSERT_(predicate)                                    \
    GLCORE_ASSERTMSG_(predicate, std::string("Assertion failed: ") + \
                                     ::std::string(#predicate))

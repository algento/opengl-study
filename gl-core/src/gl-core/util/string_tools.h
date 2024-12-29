/**
 * @file string_tools.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-28
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once

namespace glcore {

inline std::string ChangePathSeparator(const std::string& path) {
    std::string result = path;
    for (char& c : result) {
        if (c == '\\') {
            c = '/';
        }
    }
    return result;
}

inline std::string GetFileNameWithoutExtention(const std::string& path) {
    std::cout << path << std::endl;
    std::string result = ChangePathSeparator(path);
    size_t start_idx   = result.rfind('/');
    size_t end_idx     = result.rfind('.');
    return result.substr(start_idx + 1, end_idx - start_idx - 1);
}

inline std::string GetFileNameWithExtention(const std::string& path) {
    std::cout << path << std::endl;
    std::string result = ChangePathSeparator(path);
    size_t start_idx   = result.rfind('/');
    return result.substr(start_idx + 1);
}

}  // namespace glcore
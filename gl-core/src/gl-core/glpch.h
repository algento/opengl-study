/**
 * @file glpch.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
//
#include <array>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#if defined(PLATFORM_WINDOWS)
    #include <Windows.h>
#endif
#if defined(PLATFORM_LINUX) || defined(PLATFORM_APPLE)
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif
/**
 * @file texture.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-29
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */

#include <gl-core/renderer/texture.h>

namespace glcore {
Texture2D::Texture2D(uint32_t width, uint32_t height) {}
Texture2D::~Texture2D() {}

void Texture2D::SetData(void* data, uint32_t size) {}
void Texture2D::Bind() {}
void Texture2D::Unbind() {}
}  // namespace glcore
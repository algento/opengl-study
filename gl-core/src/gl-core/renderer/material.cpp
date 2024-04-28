/**
 * @file material.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */
#include "gl-core/renderer/material.h"

namespace glcore {
void Material::UseMaterial(const Shader& shader) const {
    shader.SetFloat("u_material.specular_intensity", specular_intensity_);
    shader.SetFloat("u_material.shininess", shininess_);
}
}  // namespace glcore

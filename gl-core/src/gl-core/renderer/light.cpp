/**
 * @file light.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-20
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#include "gl-core/renderer/light.h"

#include "gl-core/shader/shader.h"
namespace glcore {

void Light::UseLight(const Shader& shader) {
    shader.SetFloat3("u_light.color", color_);
    shader.SetFloat("u_light.ambient_intensity", ambient_intensity_);
    shader.SetFloat3("u_light.direction", direction_);
    shader.SetFloat("u_light.diffuse_intensity", diffuse_intensity_);
}

}  // namespace glcore

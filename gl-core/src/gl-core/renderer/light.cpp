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

#include <fmt/core.h>

#include "gl-core/shader/shader.h"
namespace glcore {

void DirectionalLight::UseLight(const Shader& shader) {
    shader.SetFloat3("u_directional_light.color", color());
    shader.SetFloat("u_directional_light.ambient_intensity",
                    ambient_intensity());
    shader.SetFloat("u_directional_light.diffuse_intensity",
                    diffuse_intensity());
    shader.SetFloat3("u_directional_light.direction", direction_);
}

uint32_t PointLight::count_ = 0;

void PointLight::UseLight(const Shader& shader) {
    std::string uniform_light = fmt::format("u_point_light[{}]", id_);
    if (id_ == count_ - 1) {
        shader.SetInt("u_num_point_lights",
                      static_cast<int>(PointLight::count_));
    }
    shader.SetFloat3(uniform_light + ".color", color());
    shader.SetFloat(uniform_light + ".ambient_intensity", ambient_intensity());
    shader.SetFloat(uniform_light + ".diffuse_intensity", diffuse_intensity());
    shader.SetFloat3(uniform_light + ".position", position_);
    shader.SetFloat3(uniform_light + ".coefficient", coeff_);
}
}  // namespace glcore

/**
 * @file material.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */
#pragma once

#include "gl-core/shader/shader.h"
namespace glcore {
class Material {
 public:
    Material() = default;

    Material(float specular_intensity, float shininess)
        : specular_intensity_(specular_intensity), shininess_(shininess) {}

    void UseMaterial(const Shader& shader) const;

    [[nodiscard]] float specular_intensity() const {
        return specular_intensity_;
    }
    [[nodiscard]] float shininess() const { return shininess_; }

 private:
    float specular_intensity_ = 0.0F;
    float shininess_          = 0.0F;
};
}  // namespace glcore
/**
 * @file light.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once

#include "glm/glm.hpp"

namespace glcore {
class Shader;
class Light {
 public:
    Light() = default;

    Light(glm::vec3 color, float abmient_intensity, glm::vec3 dierection,
          float diffuse_intensity)
        : color_(color),
          ambient_intensity_(abmient_intensity),
          direction_(dierection),
          diffuse_intensity_(diffuse_intensity) {}

    void UseLight(const Shader& shader);

    [[nodiscard]] glm::vec3 color() const { return color_; }
    [[nodiscard]] float ambient_intensity() const { return ambient_intensity_; }
    [[nodiscard]] glm::vec3 direction() const { return direction_; }
    [[nodiscard]] float diffuse_intensity() const { return diffuse_intensity_; }

 private:
    glm::vec3 color_{1.0F, 1.0F, 1.0F};
    float ambient_intensity_ = 1.0F;

    glm::vec3 direction_{0.0F, -1.0F, 0.0F};
    float diffuse_intensity_ = 0.0F;
};

}  // namespace glcore
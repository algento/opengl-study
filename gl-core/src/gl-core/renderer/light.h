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
class Light {
 public:
    Light()
        : color_(glm::vec3(1.0F, 1.0F, 1.0F)),
          ambient_intensity_(1.0F),
          diffuse_intensity_(0.0F) {}
    Light(glm::vec3 color, float abmient_intensity, float diffuse_intensity)
        : color_(color),
          ambient_intensity_(abmient_intensity),
          diffuse_intensity_(diffuse_intensity) {}

    [[nodiscard]] glm::vec3 color() const { return color_; }
    [[nodiscard]] float ambient_intensity() const { return ambient_intensity_; }
    [[nodiscard]] float diffuse_intensity() const { return diffuse_intensity_; }

 private:
    glm::vec3 color_;
    float ambient_intensity_;
    float diffuse_intensity_;
};

}  // namespace glcore
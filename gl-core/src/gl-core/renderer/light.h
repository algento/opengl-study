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

#include <glm/geometric.hpp>

#include "gl-core/shader/shader.h"

namespace glcore {
class Light {  //NOLINT
 public:
    Light(glm::vec3 color, float ambient_intensity, float diffuse_intensity)
        : color_(color),
          ambient_intensity_(ambient_intensity),
          diffuse_intensity_(diffuse_intensity) {}
    virtual ~Light() = default;

    virtual void UseLight(const Shader& shader) = 0;

    [[nodiscard]] glm::vec3 color() const { return color_; }
    [[nodiscard]] float ambient_intensity() const { return ambient_intensity_; }

    [[nodiscard]] float diffuse_intensity() const { return diffuse_intensity_; }

 private:
    glm::vec3 color_{1.0F, 1.0F, 1.0F};
    float ambient_intensity_ = 1.0F;
    float diffuse_intensity_ = 0.0F;
};

class DirectionalLight : public Light {
 public:
    DirectionalLight(glm::vec3 color, float ambient_intensity,
                     float diffuse_intensity, glm::vec3 direction)
        : Light(color, ambient_intensity, diffuse_intensity),
          direction_(direction) {}

    void UseLight(const Shader& shader) override;

    [[nodiscard]] glm::vec3 direction() const { return direction_; }

 private:
    glm::vec3 direction_{0.0F, -1.0F, 0.0F};
};

class PointLight : public Light {
 public:
    PointLight(glm::vec3 color, float ambient_intensity,
               float diffuse_intensity, glm::vec3 position,
               glm::vec3 attenuation_coeff)
        : Light(color, ambient_intensity, diffuse_intensity),
          id_(count_++),
          position_(position),
          attenuation_coeff_(attenuation_coeff) {}

    void UseLight(const Shader& shader) override;

    [[nodiscard]] glm::vec3 position() const { return position_; }
    [[nodiscard]] glm::vec3 attenuation_coeff() const {
        return attenuation_coeff_;
    }
    [[nodiscard]] uint32_t id() const { return id_; }

    static uint32_t count_;

 private:
    uint32_t id_{0};
    //* where the light is in the world
    glm::vec3 position_{0.0F, 0.0F, 0.0F};
    //* how far the light will shine (1/a x^2 + b x + c)
    glm::vec3 attenuation_coeff_{0.0F, 0.0F, 0.0F};
};

class SpotLight : public Light {
 public:
    SpotLight(glm::vec3 color, float ambient_intensity, float diffuse_intensity,
              glm::vec3 position, glm::vec3 attenuation_coeff,
              glm::vec3 direction, float cutoff_angle)
        : Light(color, ambient_intensity, diffuse_intensity),
          id_(count_++),
          position_(position),
          attenuation_coeff_(attenuation_coeff),
          direction_(glm::normalize(direction)),
          cutoff_angle_(cutoff_angle) {}

    void UseLight(const Shader& shader) override;
    void SetFlash(glm::vec3 pos, glm::vec3 dir) {
        position_  = pos;
        direction_ = dir;
    }

    [[nodiscard]] glm::vec3 position() const { return position_; }
    [[nodiscard]] glm::vec3 attenuation_coeff() const {
        return attenuation_coeff_;
    }
    [[nodiscard]] glm::vec3 direction() const { return direction_; }
    [[nodiscard]] float cutoff_angle() const { return cutoff_angle_; }
    [[nodiscard]] uint32_t id() const { return id_; }

    static uint32_t count_;

 private:
    uint32_t id_{0};
    //* where the light is in the world
    glm::vec3 position_{0.0F, 0.0F, 0.0F};
    //* how far the light will shine (1/a x^2 + b x + c)
    glm::vec3 attenuation_coeff_{0.0F, 0.0F, 0.0F};
    //* where the light is pointing
    glm::vec3 direction_{0.0F, -1.0F, 0.0F};
    //* how wide the light is
    float cutoff_angle_{0.0F};
};
}  // namespace glcore
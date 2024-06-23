/**
 * @file camera.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-05
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#include "gl-core/camera/camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

#include "gl-core/event/key_code.h"
#include "gl-core/event/mouse_code.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "gl-core/event/user_input.h"

namespace glcore {

constexpr float kDefaultFocalLength = 10.0F;
constexpr float kDeltaScale         = 0.003F;
constexpr float kZoomScale          = 0.1F;
constexpr float kMinFocalLength     = 1.0F;
constexpr float kRotationScale      = 0.8F;
constexpr float kDistanceScale      = 0.2F;
constexpr float kMaxZoomSpeed       = 100.0F;

PerspCamera::PerspCamera(float fov, float width, float height, float near,
                         float far)
    : focal_length_(kDefaultFocalLength),
      fov_(fov),
      near_(near),
      far_(far),
      viewport_width_(width),
      viewport_height_(height) {
    projection_matrix_ =
        glm::perspectiveFov(glm::radians(fov), width, height, near, far);
}

void PerspCamera::OnEvent(GuiEvent& event) {}

bool PerspCamera::OnMouseScroll(GuiEvent& event) {}

void PerspCamera::OnUpdate(float delta_time) {}

void PerspCamera::OnUpdateTmp(void* window, float delta_time) {
    const glm::vec2& mouse{UserInput::GetMouseX((GLFWwindow*)window),
                           UserInput::GetMouseY((GLFWwindow*)window)};
    // 왼쪽 Alt를 누르고 마우스를 조작하면 Camera View를 변환한다.
    if (UserInput::IsKeyPressed((GLFWwindow*)window, KeyCode::LeftAlt)) {
        glm::vec2 scaled_delta = (mouse - mouse_position_) * kDeltaScale;

        if (UserInput::IsMouseButtonPressed((GLFWwindow*)window,
                                            MouseCode::ButtonRight)) {
            MousePan(scaled_delta);
        } else if (UserInput::IsMouseButtonPressed((GLFWwindow*)window,
                                                   MouseCode::ButtonLeft)) {
            MouseRotate(scaled_delta);
        } else {
            /* do nothing */
        }
    }

    if (UserInput::IsKeyPressed((GLFWwindow*)window, KeyCode::LeftShift)) {
        const glm::vec2& mouse{UserInput::GetMouseX((GLFWwindow*)window),
                               UserInput::GetMouseY((GLFWwindow*)window)};
        glm::vec2 scaled_delta = (mouse - mouse_position_) * 0.1F;
        MouseZoom(scaled_delta.y);
    } else {
    }
    mouse_position_ = mouse;
    UpdateViewMatrix();
}

void PerspCamera::MousePan(const glm::vec2& delta) {
    auto [xSpeed, ySpeed] = PanSpeed();
    focal_point += -GetRightAxis() * delta.x * xSpeed * focal_length_;
    focal_point += GetUpAxis() * delta.y * ySpeed * focal_length_;
}

void PerspCamera::MouseRotate(const glm::vec2& delta) {
    float yaw_sign = GetUpAxis().y < 0 ? -1.0F : 1.0F;
    orientation_.y -= yaw_sign * delta.x * kRotationScale;
    orientation_.x -= delta.y * kRotationScale;
}

void PerspCamera::MouseZoom(float delta) {
    focal_length_ -= delta * ZoomSpeed() * kZoomScale;
    if (focal_length_ < kMinFocalLength) {
        focal_point += GetForwardAxis();
        focal_length_ = kMinFocalLength;
    }
}

void PerspCamera::UpdateProjectionMatrix() {
    projection_matrix_ = glm::perspectiveFov(
        glm::radians(fov_), viewport_width_, viewport_height_, near_, far_);
}

void PerspCamera::UpdateViewMatrix() {
    position_ = CalcluatePosition();

    //* orientation에서 roll을 뺀다. orientation은 $^{w}R_{c}$를 의미한다.
    auto quat = CalculateOrientation();

    //* view_matrix_는 $^{c}T_{w}$이므로 $^{w}T_{c}$를 구하고 inverse를 취한다.
    view_matrix_ = glm::inverse(glm::translate(glm::mat4(1.0F), position_) *
                                glm::toMat4(quat));

    //* Classic calculation
    /* view_matrix_ =
        glm::lookAt(position_, position_ + GetForwardAxis(), GetUpAxis()); */
}

glm::vec3 PerspCamera::GetUpAxis() const {
    //* OpenGL에서는 y축이 up이다.
    return glm::rotate(CalculateOrientation(), glm::vec3(0.0F, 1.0F, 0.0F));

    //* Classic calculation
    // return glm::normalize(glm::cross(GetRightAxis(), GetForwardAxis()));
}

glm::vec3 PerspCamera::GetRightAxis() const {
    //* OpenGL에서는 x축이 right이다.
    return glm::rotate(CalculateOrientation(), glm::vec3(1.0F, 0.0F, 0.0F));

    //* Classic calculation
    /* return glm::normalize(
        glm::cross(GetForwardAxis(), glm::vec3(0.0F, 1.0F, 0.0F))); */
}

glm::vec3 PerspCamera::GetForwardAxis() const {
    //* OpenGL에서는 -z축이 forward이다.
    return glm::rotate(CalculateOrientation(), glm::vec3(0.0F, 0.0F, -1.0F));

    //* Classic calculation
    /*  glm::vec3 front;
    float yaw   = -orientation_.y;
    float pitch = -orientation_.x;
    front.x     = sin(yaw) * cos(pitch);
    front.y     = sin(pitch);
    front.z     = -cos(yaw) * cos(pitch);
    return glm::normalize(front); */
}

glm::quat PerspCamera::CalculateOrientation() const {
    return {glm::vec3(orientation_.x, orientation_.y, 0.0F)};
}

glm::vec3 PerspCamera::CalcluatePosition() const {
    return focal_point - GetForwardAxis() * focal_length_;
}

std::pair<float, float> PerspCamera::PanSpeed() const {
    //TODO: reference
    float x       = std::min(viewport_width_ / 1000.0f, 2.4f);  //NOLINT
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.1778f;  //NOLINT

    float y       = std::min(viewport_height_ / 1000.0f, 2.4f);  //NOLINT
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.1778f;   //NOLINT

    return {xFactor, yFactor};
}

float PerspCamera::ZoomSpeed() const {
    float scaled_focal_length = std::max(focal_length_ * kDistanceScale, 0.0F);
    return std::min(scaled_focal_length * scaled_focal_length, kMaxZoomSpeed);
}
}  // namespace glcore
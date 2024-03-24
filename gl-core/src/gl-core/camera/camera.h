/**
 * @file camera.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-05
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl-core/base/noncopyable.h"

namespace glcore {
class GuiEvent;

class CameraBase : private NonCopyable {  // NOLINT
 public:
    void SetProjectionMatrix(glm::mat4 proj) { projection_matrix_ = proj; }

    void SetViewMatrix(glm::mat4 view) { view_matrix_ = view; }

    glm::mat4& view_prjection_matrix() {
        //* glm은 column-major order이므로 일반적인 행렬 곱셈 순서와 동일하게 곱해준다.
        view_projection_matrix_ = projection_matrix_ * view_matrix_;
        return view_projection_matrix_;
    }

    void SetPosition(glm::vec3 pos) {
        position_ = pos;
        UpdateViewMatrix();
    }

    void SetRotation(glm::vec3 orientation) {
        orientation_ = orientation;
        UpdateViewMatrix();
    }

    [[nodiscard]] const glm::mat4& view_matrix() const { return view_matrix_; }
    [[nodiscard]] const glm::mat4& projection_matrix() const {
        return projection_matrix_;
    }

    [[nodiscard]] const glm::vec3& orientation() const { return orientation_; }
    [[nodiscard]] const glm::vec3& position() const { return position_; }

    [[nodiscard]] glm::mat4 GetViewProjectionMatrix() const {
        return projection_matrix_ * view_matrix_;
    }

    virtual void OnUpdate(float delta_time)     = 0;
    virtual void OnEvent(GuiEvent& event)       = 0;
    virtual bool OnMouseScroll(GuiEvent& event) = 0;

 protected:
    virtual void UpdateViewMatrix()       = 0;
    virtual void UpdateProjectionMatrix() = 0;

    glm::mat4 projection_matrix_{1.0F};
    glm::mat4 view_matrix_{1.0F};
    glm::mat4 view_projection_matrix_{1.0F};

    glm::vec3 position_{0.0F};
    glm::vec3 orientation_{0.0F};
};

/**
 * @brief Prespective Camera
 * In "world space", x-axis is right, y-axis is up, and z-axis is backward w.r.t user. origin is at the center of world.
 * In "view space", x-axis is right, y-axis is up, and z-axis is backward w.r.t user. origin is at the center of camera.
 * In "device space (2D)", x-axis is right, y-axis is down. origin is top-left.
 */

class PerspCamera : public CameraBase {
 public:
    PerspCamera(float fov, float width, float height, float near, float far);

    void OnUpdate(float delta_time) override;
    void OnUpdateTmp(void* window, float delta_time);
    void OnEvent(GuiEvent& event) override;
    bool OnMouseScroll(GuiEvent& event) override;

    [[nodiscard]] glm::vec3 GetPosition() const { return position_; }
    [[nodiscard]] glm::vec3 GetOrientation() const { return orientation_; }
    [[nodiscard]] glm::vec3 GetFocalPoint() const { return focal_point; }
    [[nodiscard]] float GetFocalLength() const { return focal_length_; }

    /// @brief Get up axis in world space (the cross product of right axis and forward axis)
    [[nodiscard]] glm::vec3 GetUpAxis() const;

    /// @brief Get right axis in world space (the cross product of up axis of world space and forward axis)
    [[nodiscard]] glm::vec3 GetRightAxis() const;

    /// @brief Get forward axis in world space (the vector from camera center to the focal point)
    [[nodiscard]] glm::vec3 GetForwardAxis() const;

    [[nodiscard]] float fov() const { return fov_; }
    [[nodiscard]] float near() const { return near_; }
    [[nodiscard]] float far() const { return far_; }

    [[nodiscard]] float viewport_width() const { return viewport_width_; }
    [[nodiscard]] float viewport_height() const { return viewport_height_; }

    void SetFov(float fov) {
        fov_ = fov;
        UpdateProjectionMatrix();
    }
    void SetViewport(float width, float height) {
        viewport_width_  = width;
        viewport_height_ = height;
        UpdateProjectionMatrix();
    }
    void SetNear(float near) {
        near_ = near;
        UpdateProjectionMatrix();
    }
    void SetFar(float far) {
        far_ = far;
        UpdateProjectionMatrix();
    }

 private:
    void UpdateProjectionMatrix() override;
    void UpdateViewMatrix() override;

    /// @brief Pan speed in x and y direction, respectively.
    /// if mouse is moved to the right, camera moves to the left.
    /// if mouse is moved to the up, camera moves to the down.
    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    void MouseZoom(float delta);

    [[nodiscard]] glm::vec3 CalcluatePosition() const;
    [[nodiscard]] glm::quat CalculateOrientation() const;
    [[nodiscard]] std::pair<float, float> PanSpeed() const;
    [[nodiscard]] float ZoomSpeed() const;

    glm::vec2 mouse_position_{0.0F};

    glm::vec3 focal_point{0.0F};
    float focal_length_{0.0F};

    float fov_{45.0F};
    float near_{0.1F};
    float far_{10.0F};

    float viewport_width_{640.0F};
    float viewport_height_{480.0F};
};

}  // namespace glcore
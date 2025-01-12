/**
 * @file shadow_map.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-11
 * FrameBuffer를 사용해서 Depth Map을 구현한다.
 * https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
 * @copyright Copyright (c) Sejong Heo 2025
 * 
 */

#pragma once

#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif

namespace glcore {
class ShadowMap {
 public:
    ShadowMap(uint32_t width, uint32_t height)
        : shadow_width_(width), shadow_height_(height) {
        is_initialized_ = Init(shadow_width_, shadow_height_);
    }
    ShadowMap(const ShadowMap& other)                = delete;
    ShadowMap& operator=(const ShadowMap& other)     = delete;
    ShadowMap(ShadowMap&& other) noexcept            = delete;
    ShadowMap& operator=(ShadowMap&& other) noexcept = delete;

    bool Init(uint32_t width, uint32_t height);

    /**
     * @brief Depthmap을 Framebuffer에 쓴다.
     * 
     */
    void Write() const;

    /**
     * @brief TextureUnit에 해당하는 Texture를 읽어서 ShadowMap으로 랜더링한다.
     *        여러 개의 광원이 있는 경우, 각 광원 별로 랜더리이 필요하다.
     * 
     * @param texture_unit 
     */
    void Read(GLenum texture_unit) const;

    ~ShadowMap();

    [[nodiscard]] GLuint shadow_width() const { return shadow_width_; }
    [[nodiscard]] GLuint shadow_height() const { return shadow_height_; }

    // protected: // 상속해서 자식 클래스를 만들 것이기 때문에 protected로 선언한다.

 private:
    GLuint depth_map_FBO_ = 0;
    GLuint shadow_map_    = 0;
    bool is_initialized_  = false;
    // ViewPort의 크기를 ShadowMap의 크기와 일치시키기 위해 아래의 필드 필요하다.
    GLuint shadow_width_;
    GLuint shadow_height_;
};
}  // namespace glcore
/**
 * @file shadow_map.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-11
 * 
 * @copyright Copyright (c) Sejong Heo 2025
 * 
 */
#include "gl-core/renderer/shadow_map.h"

#include "gl-core/renderer/texture.h"

namespace glcore {
bool ShadowMap::Init(uint32_t width, uint32_t height) {
    // 1. Framebuffer 생성
    glGenFramebuffers(1, &depth_map_FBO_);

    // 2. Framebuffer 내에 DepthMap을 저장할 Texture를 생성한다.
    glGenTextures(1, &shadow_map_);
    glBindTexture(GL_TEXTURE_2D, shadow_map_);
    // Depth 포맷으로 텍스처 이미지를 생성한다. (NDC 기준: X-Y: -1~+1, Depth: 0~1)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // 2-1. 텍스처 보간 방식을 설정한다.
    // https://heinleinsgame.tistory.com/9
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 2.2. 텍스처의 Border 영역 처리 방식을 설정한다. (영역 벗어나면 단일 생상 사용)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // 2.3. 텍스처의 Border 영역 색을 설정한다.
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);

    // 3. Framebuffer를 바인드한다.(FBO_에 쓰기만 가능한 Framebuffer를 바인딩!)
    // - read/write: GL_FRAMEBUFFER, read: GL_READ_FRAMEBUFFER, write: GL_DRAW_FRAMEBUFFER
    // https://heinleinsgame.tistory.com/24
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, depth_map_FBO_);

    // 4. Framebuffer와 연관된 텍스쳐를 설정한다.
    // - attachment: DepthMap 저장하므로 GL_DEPTH_ATTACHMENT로 설정한다.
    // - textarget: DepthMap을 2D 텍스처로 저장
    // - level: MipMap 사용 안하면 0으로 설정
    // GLenum target, GLenum attachment: GL_DEPTH_ATTACHMENT, GLenum textarget, GLuint texture, GLint level);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, shadow_map_, 0);

    // 5. Framebuffer 설정
    // - Shadow Map은 Depth map만 필요하다.
    // - (별도의 컬러 랜더링 불필요) glDrawBuffer를 GL_NONE으로 설정한다.사용하면 GL_COLOR_ATTACHMENT0로 설정
    // - 쓰기만 하므로 glReadBuffer를 GL_NONE으로 설정한다.
    glDrawBuffer(GL_NONE);
    // glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glReadBuffer(GL_NONE);
    // Framebuffer 상태를 확인한다.
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer error: %u\n", Status);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return Status == GL_FRAMEBUFFER_COMPLETE;
}

void ShadowMap::Write() const {
    // depth map을 저장한 fbo를 바인딩한다.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, depth_map_FBO_);
}

void ShadowMap::Read(GLenum texture_unit) const {
    // 읽기를 원하는 텍스쳐를 활성화하고 해당 택스쳐를 shadowMap에 바인딩한다.
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, shadow_map_);
}

ShadowMap::~ShadowMap() {
    if (depth_map_FBO_ != 0u) {
        glDeleteFramebuffers(1, &depth_map_FBO_);
    }

    if (shadow_map_ != 0u) {
        glDeleteTextures(1, &shadow_map_);
    }
}
}  // namespace glcore
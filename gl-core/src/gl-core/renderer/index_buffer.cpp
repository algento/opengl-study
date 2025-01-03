/**
 * @file index_buffer.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#include "gl-core/renderer/index_buffer.h"

#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif
namespace glcore {
IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) {
    // OpenGL Buffer를 생성하고 바인딩한다.
#if GL_VERSION >= 0x1F05
    glCreateBuffers(1, &id_);
    glNamedBufferData(id_, size, indices, GL_STATIC_DRAW);
#endif
    // Index Buffer에는 GL_ELEMENT_ARRAY_BUFFER를 사용한다.
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<uint32_t>(count * sizeof(uint32_t)), indices,
                 GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { Release(); }

void IndexBuffer::Release() {
    if (id_ != 0) {
        glDeleteBuffers(1, &id_);
    }
    id_ = 0;
}

void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }
void IndexBuffer::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices,
                                                 uint32_t count) {
    return std::make_shared<IndexBuffer>(indices, count);
}
}  // namespace glcore

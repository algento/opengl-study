/**
 * @file vertex_buffer.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#include "gl-core/renderer/vertex_buffer.h"

#include <glad/glad.h>

#include <memory>
namespace glcore {
VertexBuffer::VertexBuffer(uint32_t size) {
    // OpenGL Buffer를 생성하고 바인딩한다.
#if GL_VERSION >= 0x1F05
    glCreateBuffers(1, &id_);
    glNamedBufferData(id_, size, nullptr, GL_DYNAMIC_DRAW);
#endif
    // Vertex Buffer에는 GL_ARRAY_BUFFER를 사용한다.
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(void* vertices_data, uint32_t size) {
    // OpenGL Buffer를 생성하고 바인딩한다.
#if GL_VERSION >= 0x1F05
    glCreateBuffers(1, &id_);
    glNamedBufferData(id_, size, vertices_data, GL_DYNAMIC_DRAW);
#endif
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, vertices_data, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &id_); }

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }
void VertexBuffer::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::SetData(const void* data, uint32_t size) const {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    return std::make_shared<VertexBuffer>(size);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(void* vertices_data,
                                                   uint32_t size) {
    return std::make_shared<VertexBuffer>(vertices_data, size);
}

}  // namespace glcore
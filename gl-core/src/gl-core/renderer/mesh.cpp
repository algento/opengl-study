/**
 * @file mesh.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */
#include "gl-core/renderer/mesh.h"

#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif

#include "gl-core/renderer/buffer_layout.h"
#include "gl-core/renderer/index_buffer.h"

namespace glcore {
void Mesh::Create(float* vertex_buffer_data, uint32_t vertex_buffer_size,
                  const GlBufferLayout& layout, uint32_t* index_buffer_data,
                  uint32_t index_count) {
    vertex_buffer_ =
        VertexBuffer::Create((void*)vertex_buffer_data, vertex_buffer_size);
    vertex_buffer_->SetLayout(layout);

    index_buffer_ = IndexBuffer::Create(index_buffer_data, index_count);

    vertex_array_ = VertexArray::Create();
    vertex_array_->SetVertexBuffer(vertex_buffer_);
    vertex_array_->SetIndexBuffer(index_buffer_);
    index_count_  = index_count;
    vertex_count_ = vertex_buffer_size / layout.GetStride();

    vertex_buffer_->Unbind();
    index_buffer_->Unbind();
    vertex_array_->Unbind();
}

void Mesh::Render() const {
    vertex_array_->Bind();
    index_buffer_->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Mesh::Release() {
    if (vertex_buffer_ != nullptr) {
        vertex_buffer_->Release();
    }
    if (index_buffer_ != nullptr) {
        index_buffer_->Release();
    }
    if (vertex_array_ != nullptr) {
        vertex_array_->Release();
    }
}
}  // namespace glcore
/**
 * @file vertex_array.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#include "gl-core/renderer/vertex_array.h"

#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif

#include "gl-core/renderer/buffer_layout.h"

namespace {}  // namespace

namespace glcore {
VertexArray::VertexArray() { glGenVertexArrays(1, &id_); }

VertexArray::~VertexArray() { Release(); }

void VertexArray::Release() {
    if (id_ != 0) {
        glDeleteVertexArrays(1, &id_);
    }
    id_ = 0;
}

void VertexArray::Bind() const { glBindVertexArray(id_); }
void VertexArray::Unbind() { glBindVertexArray(0); }

void VertexArray::SetVertexBuffer(
    // #lizard forgives the complexity
    const std::shared_ptr<VertexBuffer>& vertex_buffer) {
    Bind();
    vertex_buffer->Bind();
    const auto& layout = vertex_buffer->GetLayout();

    uint32_t index{0};
    for (const auto& element : layout) {
        auto count      = element.GetComponentCount();
        auto normalized = static_cast<GLboolean>(element.is_normalized());
        auto stride     = layout.GetStride();

        const void* offset = nullptr;
        switch (element.GetAttribType()) {
            case GlBufferElement::AttribType::kNone:
                std::cout << "No Shader data type" << '\n';
                break;
            case GlBufferElement::AttribType::kFloat:
                offset = (const void*)element.offset();  //NOLINT
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, count, GL_FLOAT, normalized,
                                      stride, offset);
                index++;
                break;
            case GlBufferElement::AttribType::kInt:
                offset = (const void*)element.offset();  //NOLINT
                glEnableVertexAttribArray(index);
                glVertexAttribIPointer(index, count, GL_INT, stride, offset);
                index++;
                break;
            case GlBufferElement::AttribType::kMat:
                glEnableVertexAttribArray(index);
                for (int32_t i = 0; i < count; i++) {
                    offset = (const void*)(element.offset() +  //NOLINT
                                           sizeof(float) * count * i);
                    glVertexAttribPointer(index, count, GL_FLOAT, normalized,
                                          stride, offset);
                    // 보통 행렬은 정점에 공통으로 적용하므로 인스턴스로 지정
                    glVertexAttribDivisor(index, 1);
                    index++;
                }
                break;
        }
    }
    vertex_buffer_ = vertex_buffer;
}

void VertexArray::SetIndexBuffer(
    const std::shared_ptr<IndexBuffer>& index_buffer) {
    Bind();
    index_buffer->Bind();
    index_buffer_ = index_buffer;
}

std::shared_ptr<VertexArray> VertexArray::Create() {
    return std::make_shared<VertexArray>();
}
}  // namespace glcore
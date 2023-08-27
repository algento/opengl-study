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

#include "gl-core/renderer/buffer_types.h"

namespace {}  // namespace

namespace glcore {
GLenum GetOpenGLDataType(GlDataType type) {
    // #lizard forgives the complexity
    switch (type) {
        case GlDataType::Int:
        case GlDataType::Int2:
        case GlDataType::Int3:
        case GlDataType::Int4:
            return GL_INT;
        case GlDataType::Float:
        case GlDataType::Float2:
        case GlDataType::Float3:
        case GlDataType::Float4:
        case GlDataType::Mat3:
        case GlDataType::Mat4:
            return GL_FLOAT;
        case GlDataType::Uint:
        case GlDataType::Uint2:
        case GlDataType::Uint3:
        case GlDataType::Uint4:
            return GL_UNSIGNED_INT;
        case GlDataType::Bool:  // TODO: Check bool case
            return GL_BOOL;
        default:
            break;
    }
    return 0;
}

VertexArray::VertexArray() { glGenVertexArrays(1, &id_); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &id_); }

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
        auto type       = GetOpenGLDataType(element.type);
        auto normalized = static_cast<GLboolean>(element.normalized);
        auto stride     = layout.GetStride();

        const void* offset = nullptr;
        switch (element.type) {
            case GlDataType::None:
                std::cout << "No Shader data type" << std::endl;
                break;
            case GlDataType::Float:
            case GlDataType::Float2:
            case GlDataType::Float3:
            case GlDataType::Float4:
                offset = (const void*)element.offset;  //NOLINT
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, count, type, normalized, stride,
                                      offset);
                index++;
                break;
            case GlDataType::Int:
            case GlDataType::Int2:
            case GlDataType::Int3:
            case GlDataType::Int4:
            case GlDataType::Uint:
            case GlDataType::Uint2:
            case GlDataType::Uint3:
            case GlDataType::Uint4:
            case GlDataType::Bool:
                offset = (const void*)element.offset;  //NOLINT
                glEnableVertexAttribArray(index);
                glVertexAttribIPointer(index, count, type, stride, offset);
                index++;
                break;
            case GlDataType::Mat3:
            case GlDataType::Mat4:
                glEnableVertexAttribArray(index);
                for (int32_t i = 0; i < count; i++) {
                    offset = (const void*)(element.offset +  //NOLINT
                                           sizeof(float) * count * i);
                    glVertexAttribPointer(index, count, type, normalized,
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
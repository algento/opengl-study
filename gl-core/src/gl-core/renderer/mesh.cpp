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
    glDrawElements(GL_TRIANGLES, (int32_t)index_count_, GL_UNSIGNED_INT,
                   nullptr);
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

void TriangleMesh::Create(std::vector<TriangleMesh::VertexInput>& vertices,
                          std::vector<uint32_t>& indices) {
    std::vector<TriangleMesh::VertexLayout> vertices_layout(vertices.size());

    for (size_t i = 0; i < vertices.size(); i++) {
        vertices_layout[i].position = vertices[i].position;
        vertices_layout[i].texcoord = vertices[i].texcoord;
        vertices_layout[i].color    = vertices[i].color;
    }

    CalculateAverageNormals(vertices, indices, vertices_layout);
    Mesh::Create(
        (float*)vertices_layout.data(),
        (uint32_t)vertices_layout.size() * sizeof(TriangleMesh::VertexLayout),
        GlBufferLayout({{"a_position", GlBufferElement::DataType::kFloat3},
                        {"a_texcoord", GlBufferElement::DataType::kFloat2},
                        {"a_color", GlBufferElement::DataType::kFloat4},
                        {"a_normal", GlBufferElement::DataType::kFloat3}}),
        indices.data(), indices.size());
}

void TriangleMesh::CalculateAverageNormals(
    std::vector<TriangleMesh::VertexInput>& vertices,
    std::vector<uint32_t>& indices,
    std::vector<TriangleMesh::VertexLayout>& vertices_layout) {
    size_t indice_count = indices.size();

    for (size_t i = 0; i < indice_count; i += 3) {
        uint32_t index0 = indices[i];
        uint32_t index1 = indices[i + 1];
        uint32_t index2 = indices[i + 2];

        glm::vec3 v01 = vertices[index1].position - vertices[index0].position;
        glm::vec3 v02 = vertices[index2].position - vertices[index0].position;

        glm::vec3 normal = glm::normalize(glm::cross(v01, v02));
        vertices_layout[index0].normal += normal;
        vertices_layout[index1].normal += normal;
        vertices_layout[index2].normal += normal;
    }

    for (auto& vertex : vertices_layout) {
        vertex.normal = glm::normalize(vertex.normal);
    }
}

}  // namespace glcore
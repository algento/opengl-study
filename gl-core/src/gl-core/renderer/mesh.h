/**
 * @file mesh.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-27
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */

#pragma once
#include <glm/glm.hpp>

#include "gl-core/renderer/buffer_layout.h"
#include "gl-core/renderer/index_buffer.h"
#include "gl-core/renderer/vertex_array.h"
#include "gl-core/renderer/vertex_buffer.h"

namespace glcore {

// TODO: Seperate Mesh and MeshRenderer?
class Mesh {
 public:
    void Create(float* vertex_buffer_data, uint32_t vertex_buffer_size,
                const GlBufferLayout& layout, uint32_t* index_buffer_data,
                uint32_t index_count);
    void Render() const;
    void Release();

    std::shared_ptr<VertexArray> vertex_array_{nullptr};

 protected:
    std::shared_ptr<VertexBuffer> vertex_buffer_{nullptr};
    std::shared_ptr<IndexBuffer> index_buffer_{nullptr};
    uint32_t vertex_count_{0};
    uint32_t index_count_{0};
};

class TriangleMesh : public Mesh {
 public:
#pragma pack(push, 1)
    struct VertexInput {
        glm::vec3 position{};
        glm::vec2 texcoord{};
        glm::vec4 color{};
        glm::vec3 normal{};
    };
#pragma pack(pop)
    void Create(std::vector<VertexInput>& vertices,
                std::vector<uint32_t>& indices, bool avg_normal = true);

 private:
    static void CalculateAverageNormals(std::vector<VertexInput>& vertices,
                                        std::vector<uint32_t>& indices);
};
}  // namespace glcore
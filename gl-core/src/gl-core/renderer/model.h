/**
 * @file model.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */
#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "gl-core/renderer/mesh.h"
#include "gl-core/renderer/texture.h"

namespace glcore {
//* Mesh Group이라는 이름도 좋다.
//* - Recursive하게 모델을 로드하고 렌더링하는 클래스
class Model {
 public:
    void Load(const std::string& path);
    void Render();
    void Clear();

 private:
    //* aiNode는 특정 노드의 데이트를 가리키며, aiScene는 모든 노드들의 데이터를 가지고 있다.
    //* aiMesh는 실제 메쉬 데이터를 가지고 있다.
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessMaterials(const aiScene* scene);
    // void ProcessTexture(aiMaterial* material, aiTextureType type,
    //                     const std::string& typeName);
    // std::vector<unsigned int> textures_loaded_;
    // std::vector<unsigned int> indices_;
    // std::vector<float> vertices_;
    // std::vector<float> normals_;
    // std::vector<float> tex_coords_;
    // std::vector<unsigned int> indices_count_;
    // unsigned int vao_{0};
    // unsigned int vbo_{0};
    // unsigned int ebo_{0};
    // unsigned int texture_{0};
    // unsigned int indices_count_total_{0};
    // bool is_loaded_{false};
    // Assimp::Importer importer_;
    // const aiScene* scene_;
    // std::string directory_;

    //* Mesh, Texture를 담는 리스트들
    //* - 다수의 메쉬들이 텍스쳐 리스트에 있는 텍스쳐를 가져와서 랜더링한다.
    //* - 게임 엔진처럼 복잡한 시스템을 만든다면, 메쉬와 텍스쳐를 이렇게 함께 담지 않고 별도로 텍스쳐를 관리하고 해당 텍스쳐를 참조하는 ID를 이용해서 메쉬와 연결하는 것이 좋다.
    std::vector<Mesh*> mesh_lists_;
    std::vector<Texture*> texture_lists_;
    std::vector<uint32_t> mesh_to_texture_;
};
}  // namespace glcore
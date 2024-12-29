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
//* https://learnopengl.com/Model-Loading/Assimp
class Model {
 public:
    /**
    * @brief path에 있는 파일을 읽어서 모델을 로딩힌다.
    * 
    * @param path 
    */
    void Load(const std::string& path);
    void Render();
    void Clear();

 private:
    /**
     * @brief 재귀호출되며 scene이 가지고 있는 node를 처리하는 함수
     * 
     * @param node: assimp에서 특정 노드의 데이터를 가지고 있는 클래스
     * @param scene: assimp에서 scene (루트 노드, 메쉬 리스트, 머티리얼 리스트)의 정보를 가지고 있는 클래스
     */
    void ProcessNode(aiNode* node, const aiScene* scene);

    /**
     * @brief scene이 가지고 있는 mesh를 처리하는 함수, mesh 내부에 있는 정점과 인덱스 정보를 기반으로 처리함.
     * 
     * @param mesh:실제 메쉬 데이터를 가지고 있는 클래스
     */
    void ProcessMesh(aiMesh* mesh);

    /**
     * @brief scene이 가지고 있는 material을 로드하는 함수
     * 
     * @param scene 
     */
    void LoadMaterials(const aiScene* scene);

    //* Mesh, Texture를 담는 리스트들
    //* - 다수의 메쉬들이 텍스쳐 리스트에 있는 텍스쳐를 가져와서 랜더링한다.
    //* - 게임 엔진처럼 복잡한 시스템을 만든다면, 메쉬와 텍스쳐를 이렇게 함께 담지 않고 별도로 텍스쳐를 관리하고 해당 텍스쳐를 참조하는 ID를 이용해서 메쉬와 연결하는 것이 좋다.
    std::string name_;
    std::string texture_directory_ = "/gl-sandbox/assets/textures";
    std::vector<std::unique_ptr<TriangleMesh>> mesh_lists_;
    std::vector<std::unique_ptr<Texture>> texture_lists_;
    std::vector<uint32_t> mesh_to_material_indices_;
};
}  // namespace glcore
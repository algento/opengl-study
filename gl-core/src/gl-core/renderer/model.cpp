/**
 * @file model.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) Sejong Heo 2024
 * 
 */
#include "gl-core/renderer/model.h"

#include <assimp/material.h>
#include <assimp/types.h>

#include <iostream>
#include <memory>
#include <string>

#include <assimp/Importer.hpp>
#include <glm/fwd.hpp>

#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif

#include "gl-core/renderer/mesh.h"
#include "gl-core/renderer/texture.h"
#include "gl-core/util/string_tools.h"

namespace glcore {
void Model::Load(const std::string& path) {
    Assimp::Importer importer;

    // - aiProcess_Triangulate: 삼각형으로 변환한다. (간단한 모델로 처리하기 위해)
    // - aiProcess_FlipUVs: UV 좌표를 뒤집는다. 일반적인 이미지 파일과 OpenGL 텍스처 좌표계의 y축이 반대방향이기 때문이다.
    // - aiProcess_JoinIdenticalVertices: 중복된 정점을 하나로 합친다.
    // - aiProcess_GenSmoothNormals: 정점의 normal을 생성한다.

    name_                = GetFileNameWithoutExtention(path);
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                  aiProcess_JoinIdenticalVertices);

    if (scene == nullptr) {
        std::cout << "Model " << path
                  << "failed to load: " << importer.GetErrorString() << "\n";
        return;
    }
    std::cout << "Model " << path << " successed to load\n";

    // 루트 노드부터 시작하여 재귀적으로 노드를 처리한다.
    ProcessNode(scene->mRootNode, scene);

    // scene에 있는 material을 처리한다.
    LoadMaterials(scene);
}

void Model::Render() {
    for (size_t i = 0; i < mesh_lists_.size(); i++) {
        auto material_index = mesh_to_material_indices_[i];
        if (material_index < texture_lists_.size() &&
            texture_lists_[material_index] != nullptr) {
            texture_lists_[material_index]->Bind();
        }
        mesh_lists_[i]->Render();
    }
    Texture::Unbind();
}

void Model::Clear() {
    mesh_lists_.clear();
    texture_lists_.clear();
    mesh_to_material_indices_.clear();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {  //NOLINT
    // node에 있는 mesh를 로드한다.
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        // node->mMeshes[i] : scene이 가지고 있는 메시 리스트의 인덱스
        // 실제 메시 리스트는 scene에 저장되어있기 때문에 이렇게 참조하게 된다.
        ProcessMesh(scene->mMeshes[node->mMeshes[i]]);
    }

    // 자식 노들를 순환하면서 로드한다.
    for (size_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Model::ProcessMesh(aiMesh* mesh) {
    // VertexBuffer
    std::vector<TriangleMesh::VertexInput> vertices;
    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        TriangleMesh::VertexInput vertex_input;
        // position
        vertex_input.position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                                 mesh->mVertices[i].z};

        if (mesh->HasNormals()) {
            // normal (aiProcess_GenSmoothNormals를 적용했기 때문에 값이 존재함.
            //! "-"" 방향이 맞나?
            vertex_input.normal = {-mesh->mNormals[i].x, -mesh->mNormals[i].y,
                                   -mesh->mNormals[i].z};
        }
        // color
        vertex_input.color = {0.0F, 0.0F, 0.0F, 0.0F};
        // texture coordinate
        if (mesh->mTextureCoords[0] != nullptr) {
            vertex_input.texcoord = {mesh->mTextureCoords[0][i].x,
                                     mesh->mTextureCoords[0][i].y};
        } else {
            vertex_input.texcoord = {0.0F, 0.0F};
        }
        vertices.push_back(vertex_input);
    }
    // IndexBuffer
    std::vector<unsigned int> indices;
    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    auto triangle_mesh = std::make_unique<TriangleMesh>();

    triangle_mesh->Create(vertices, indices, !mesh->HasNormals());

    mesh_lists_.push_back(std::move(triangle_mesh));
    mesh_to_material_indices_.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene) {
    texture_lists_.resize(scene->mNumMaterials);
    for (size_t i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];
        texture_lists_[i]    = nullptr;
        // diffuse type의 텍스쳐가 존재한다면 로드한다.(빛이 비치면 컬러가 나타나는 것을 표현하는 텍스쳐 타입)
        if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0u) {
            aiString texture_path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) ==
                AI_SUCCESS) {
                // Assimp에서 로딩한 모델이 텍스처에 대해서 상대 경로를 사용하지 않는 경우를 처리하기 위해서 텍스쳐 경로를 재설정한다.
                std::string filename =
                    GetFileNameWithExtention(std::string(texture_path.C_Str()));
                std::string texture_filepath = std::string(ROOT_DIR) +
                                               texture_directory_ + "/" +
                                               name_ + "/" + filename;

                texture_lists_[i] = std::make_unique<Texture2D>(TextureImage(
                    GetFileNameWithoutExtention(filename), texture_filepath));
            }
        }
        // diffuse type의 텍스쳐가 존재하지 않는다면 기본 텍스쳐를 로드한다.
        if (texture_lists_[i] == nullptr) {
            texture_lists_[i] = std::make_unique<Texture2D>(TextureImage(
                "plain_texture",
                std::string(ROOT_DIR) + texture_directory_ + "/plain.png"));
        }
    }
}

}  // namespace glcore
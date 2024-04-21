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

namespace glcore {
void Model::Load(const std::string& path) {}
void Model::Render() {}
void Model::Clear() {}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {}
void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {}
void Model::ProcessMaterials(const aiScene* scene) {}

}  // namespace glcore
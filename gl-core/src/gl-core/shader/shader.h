/**
 * @file shader.h
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */

#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glcore {
class Shader {
 public:
    explicit Shader(const std::string& file_path);
    Shader(std::string name, const std::string& vertex_shader_source,
           const std::string& fragment_shader_source);
    ~Shader() { glDeleteProgram(id_); }
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&)                 = default;
    Shader& operator=(Shader&&)      = default;

    [[nodiscard]] GLuint GetId() const { return id_; }

    void Bind() const { glUseProgram(id_); }
    static void Unbind() { glUseProgram(0); }

    void SetInt(const std::string& name, int32_t value) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniform1i(location, value);
    }

    void SetUint(const std::string& name, uint32_t value) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniform1ui(location, value);
    }

    void SetIntArray(const std::string& name, int32_t* values,
                     uint32_t count) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniform1iv(location, (GLsizei)count, values);
    }

    void SetUintArray(const std::string& name, uint32_t* values,
                      uint32_t count) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniform1uiv(location, (GLsizei)count, values);
    }

    void SetFloat(const std::string& name, float value) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniform1f(location, value);
    }

    void SetFloat2(const std::string& name, const glm::vec2& value) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void SetFloat3(const std::string& name, const glm::vec3& value) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void SetFloat4(const std::string& name, const glm::vec4& value) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void SetMat3(const std::string& name, const glm::mat3& value) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void SetMat4(const std::string& name, const glm::mat4& value) const {
        GLint location = glGetUniformLocation(id_, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

 private:
    static std::unordered_map<GLuint, std::string> Load(
        const std::string& file_path);

    void Compile(
        std::unordered_map<GLuint, std::string>& pipeline_shader_sources);

    GLuint id_{0};
    std::string name_;
};

class ShaderMap {
 public:
    ShaderMap()                         = default;
    ~ShaderMap()                        = default;
    ShaderMap(const ShaderMap&)         = delete;
    Shader& operator=(const ShaderMap&) = delete;
    ShaderMap(ShaderMap&&)              = default;
    ShaderMap& operator=(ShaderMap&&)   = default;

    void Load(const std::string& file_path);
    void Load(const std::string& name, const std::string& file_path);

    void Add(const std::string& name, const Shader& shader);
    void Add(const std::string& name, const std::string& vertex_shader_source,
             const std::string& fragment_shader_source);
    void Add(const std::string& name, const std::string& vertex_shader_source,
             const std::string& fragment_shader_source,
             const std::string& geometry_shader_source);

    [[nodiscard]] Shader* Get(const std::string& name);
    [[nodiscard]] const Shader* Get(const std::string& name) const;
    [[nodiscard]] bool Exists(const std::string& name) const;

 private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_;
};
}  // namespace glcore
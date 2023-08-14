/**
 * @file shader.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */

#include "shader.h"

namespace {
inline GLenum ShaderTypeFromString(const std::string& type) {
    if (type == "vertex") {
        return GL_VERTEX_SHADER;
    }
    if (type == "fragment") {
        return GL_FRAGMENT_SHADER;
    }

    return 0;
}
std::string ReadFile(const std::string& file_path) {
    std::string result;
    std::ifstream ifs(file_path, std::ios::in | std::ios::binary);

    if (!ifs.is_open()) {
        std::cout << "Cannot find file!" << std::endl;
        return "";
    }
    if (ifs) {
        // 위치 지정자를 파일 끝으로 옮긴다.
        ifs.seekg(0, std::ios::end);

        auto size = ifs.tellg();
        if (size != -1) {
            // 그리고 그 위치를 읽어서 파일의 크기만큼의 string을 생성한다.
            result.resize(size);
            // 파일 스트림의 포인터를 파일의 시작으로 변경한다.
            ifs.seekg(0, std::ios::beg);
            // 파일의 내용을 읽어들인다.
            ifs.read(result.data(), (std::streamsize)result.size());
            // 파일 스트림을 닫는다.
            ifs.close();
        } else {
            std::cout << "Could not read from file " << file_path << std::endl;
            assert(false);
        }

    } else {
        std::cout << "Could not open file " << file_path << std::endl;
        assert(false);
    }
    return result;
}
}  // namespace

namespace glcore {
Shader::Shader(const std::string& file_path) {
    auto pipeline_shaders = Load(file_path);
    Compile(pipeline_shaders);

    // find last of '/' or '\'
    auto last_slash = file_path.find_last_of("/\\");
    // move to next character
    last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
    // find last of '.'
    auto last_dot = file_path.rfind('.');
    // calculate length of file name
    auto count = (last_dot == std::string::npos) ? file_path.size() - last_slash
                                                 : last_dot - last_slash;
    // get string from the last_slash to the last_dot
    name_ = file_path.substr(last_slash, count);
}

Shader::Shader(std::string name, const std::string& vertex_shader_source,
               const std::string& fragment_shader_source)
    : name_(std::move(name)) {
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER]   = vertex_shader_source;
    sources[GL_FRAGMENT_SHADER] = fragment_shader_source;
    Compile(sources);
}

std::unordered_map<GLuint, std::string> Shader::Load(
    const std::string& file_path) {
    std::string source = ReadFile(file_path);
    std::unordered_map<GLenum, std::string> pipeline_shader_sources;

    // #type <shader_type>으로 파일 내 쉐이더 타입을 확인한다.
    const char* token_type   = "#type";
    size_t token_type_length = strlen(token_type);

    // 쉐이더 타입 토큰 시작점 검색
    size_t pos = source.find(token_type, 0);
    while (pos != std::string::npos) {
        // 쉐이더 타입 토큰 종료점 검색
        size_t eol = source.find_first_of("\r\n", pos);
        if (eol == std::string::npos) {  // 토큰 다음 타입이 없는 경우 예외 처리
            std::cout << "Syntax error!" << std::endl;
            assert(false);
        }

        // 쉐이더 타입 체크
        size_t begin     = pos + token_type_length + 1;
        std::string type = source.substr(begin, eol - begin);
        if (ShaderTypeFromString(type) == 0) {
            // 토큰 다음 타입이 이상한 경우 예외 처리
            std::cout << "Invalid shader type specified" << std::endl;
            assert(false);
        }

        // 쉐이더 코드 읽기
        size_t next_line_pos = source.find_first_not_of("\r\n", eol);
        if (next_line_pos == std::string::npos) {
            std::cout << "Syntax error!" << std::endl;
            assert(false);
        }

        pos = source.find(token_type,
                          next_line_pos);  // 다음 쉐이더 타입 선언 탐색

        pipeline_shader_sources[ShaderTypeFromString(type)] =
            (pos == std::string::npos)
                ? source.substr(next_line_pos)
                : source.substr(next_line_pos, pos - next_line_pos);
    }
    return pipeline_shader_sources;
}

void Shader::Compile(
    std::unordered_map<GLuint, std::string>& pipeline_shader_sources) {
    auto program = glCreateProgram();
    std::array<uint32_t, 2> shader_ids{0, 0};  // vertex, fragment only
    int32_t index = 0;

    for (auto& kv : pipeline_shader_sources) {
        GLenum type               = kv.first;
        const std::string& source = kv.second;
        const char* src           = source.c_str();

        GLuint shader_id = glCreateShader(type);
        glShaderSource(shader_id, 1, &src, nullptr);
        glCompileShader(shader_id);

        // check compile error
        GLint isCompiled = 0;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> info_log(maxLength);  // NULL 문자 포함
            glGetShaderInfoLog(shader_id, maxLength, &maxLength,
                               info_log.data());
            glDeleteShader(shader_id);

            std::cout << "Shader compilation failure!\n"
                      << info_log.data() << std::endl;
            assert(false);
            break;
        }

        glAttachShader(program, shader_id);
        shader_ids[index++] = shader_id;  //NOLINT
    }
    id_ = program;
    glLinkProgram(id_);

    // check link error
    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int32_t*)&is_linked);
    glValidateProgram(id_);
    if (is_linked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> info_log(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, info_log.data());

        glDeleteProgram(id_);
        for (auto id : shader_ids) {
            glDeleteShader(id);
        }
        std::cout << "Shader link failure!\n" << info_log.data() << std::endl;
        assert(false);
    }

    // detach and delete shaders
    for (auto id : shader_ids) {
        glDetachShader(program, id);
        glDeleteShader(id);
    }
}
}  // namespace glcore
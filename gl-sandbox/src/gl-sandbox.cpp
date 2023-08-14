/**
 * @file gl-sandbox.cpp
 * @author Sejong Heo (tromberx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) Sejong Heo 2023
 * 
 */
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "gl-core/glpch.h"
#include "gl-core/renderer/vertex_array.h"
#include "gl-core/renderer/vertex_buffer.h"
#include "gl-core/shader/shader.h"

uint32_t CompileShader(uint32_t type, const std::string& source) {
    uint32_t id     = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int32_t result = 0;
    //* i: integer, v: vector
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int32_t length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // char message[length]; //! 이렇게 하면 오류발생, 힙에 생성해서 해결할 수도 있지만 스택에 생성하려면 이렇게 해야한다.
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader! "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "\n";
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

uint32_t CreateShader(const std::string& vertexShader,
                      const std::string& fragmentShader) {
    uint32_t program = glCreateProgram();
    uint32_t vs      = CompileShader(GL_VERTEX_SHADER, vertexShader);
    uint32_t fs      = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    //* glDetachShader를 실행할 수도 반드시 필요한 것은 아니다.
    //* 또한 이것을 실행하면, GPU에서 실행되는 Shader에 대한 디버깅 정보를 잃는다

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int32_t main(int32_t argc, char** argv) {  //NOLINT
    GLFWwindow* window = nullptr;

    /* (GLFW) GLFW 초기화*/
    if (glfwInit() == GLFW_FALSE) {
        return -1;
    }

    /* (GLFW) 윈도우 힌트 지정 */
    glfwWindowHint(GLFW_SAMPLES, 4);  // 4x Antialiasing
#if defined(PLATFORM_WINDOWS)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPACT_PROFILE);
#elif defined(PLATFORM_UNIX)
#elif defined(PLATFORM_APPLE)
    /* To use OpenGL 4.1 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE);  // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* (GLAD) GLAD 초기화 */
    gladLoadGL();
    // int32_t status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // if (status == 0) {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }

    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /* VBO -------------------------------------------------------------------*/
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // left
        0.0f,  0.5f,  0.0f,  // top
        0.5f,  -0.5f, 0.0f   // right
    };
    auto vbo = glcore::VertexBuffer::Create((void*)vertices, 9 * sizeof(float));
    vbo->SetLayout({{"a_Position", glcore::GlDataType::Float3}});

    /* VAO -------------------------------------------------------------------*/
    auto vao = glcore::VertexArray::Create();
    vao->SetVertexBuffer(vbo);

    /* Shader 설정 및 컴파일 ----------------------------------------------------*/
    glcore::Shader shader("../gl-sandbox/assets/shaders/basic.glsl");
    shader.Bind();

    /* 랜더링 루프 --------------------------------------------------------------*/
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        /* 배경색 설정 */
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // dark grey

        /* 프레임 버퍼를 배경색으로 최기화 */
        glClear(GL_COLOR_BUFFER_BIT);

        /* OpenGL이 해당 vao를 사용하도록, vao를 바인딩 */
        vao->Bind();

        /* 삼각형 그리기*/
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* (GLFW) front and back buffers를 스왑 */
        glfwSwapBuffers(window);

        /* (GLFW) 프로세스 이벤트 폴링 */
        glfwPollEvents();
    }

    /* (GLFW) 윈도우 파괴 및 종료*/
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
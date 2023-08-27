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
#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif

#include "gl-core/glpch.h"
#include "gl-core/renderer/index_buffer.h"
#include "gl-core/renderer/vertex_array.h"
#include "gl-core/renderer/vertex_buffer.h"
#include "gl-core/shader/shader.h"

int32_t main(int32_t argc, char** argv) {  //NOLINT
    GLFWwindow* window = nullptr;

    /* (GLFW) GLFW 초기화*/
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
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

    /* (GLFW) 윈도우 생성 */
    window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* (GLAD) GLAD 초기화 */
#if GLCORE_USE_GLEW
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW initialisation failed!"
                  << "\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
#else
    gladLoadGL();
    // int32_t status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // if (status == 0) {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }
#endif

    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /* VBO -------------------------------------------------------------------*/
    // r3 ----- r2
    // |         |
    // r0 ----- r1
    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // r0
        0.5f, -0.5f, 0.0f, // r1
        0.5f, 0.5f, 0.0f, // r2
        -0.5f, 0.5f, 0.0f // r3
    };
    // clang-format on
    auto vbo = glcore::VertexBuffer::Create((void*)vertices, sizeof(vertices));
    vbo->SetLayout({{"a_Position", glcore::GlDataType::Float3}});

    uint32_t indices[] = {
        0, 1, 2,  // triangle 1, counter-clockwise
        2, 3, 0   // triangle 2, counter-clockwise

    };
    auto ibo = glcore::IndexBuffer::Create(indices, 6);
    /* VAO -------------------------------------------------------------------*/
    auto vao = glcore::VertexArray::Create();
    vao->SetVertexBuffer(vbo);
    vao->SetIndexBuffer(ibo);

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

        /* 사각형 그리기 그리기*/
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
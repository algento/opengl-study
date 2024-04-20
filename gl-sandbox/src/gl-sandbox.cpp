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

#include <iostream>

#include "gl-core/renderer/texture.h"
#include "gl-core/renderer/texture_image.h"

#if GLCORE_USE_GLEW
    #include <GL/glew.h>
#else
    #include <glad/glad.h>
#endif

#include "gl-core/camera/camera.h"
#include "gl-core/renderer/buffer_layout.h"
#include "gl-core/renderer/light.h"
#include "gl-core/renderer/mesh.h"
#include "gl-core/shader/shader.h"

//NOLINTBEGIN
double delta_time = 0.0F;
double last_time  = 0.0F;
//NOLINTEND

std::shared_ptr<glcore::Mesh> CreateRectangle() {
    /* VBO -------------------------------------------------------------------*/
    // r3 ----- r2
    // |         |
    // r0 ----- r1
    // clang-format off
    float vertices[] = {
        -0.5F, -0.5F, 0.0F,  0.0F, 0.0F,// r0
        0.5F, -0.5F, 0.0F, 1.0F, 0.0F,// r1
        0.5F, 0.5F, 0.0F, 1.0F, 1.0F, // r2
        -0.5F, 0.5F, 0.0F, 0.0F, 1.0F // r3
    };
    uint32_t indices[] = {
        0, 1, 2,  // triangle 1, counter-clockwise
        2, 3, 0   // triangle 2, counter-clockwise

    };
    auto layout = glcore::GlBufferLayout({{"a_position", glcore::GlBufferElement::DataType::kFloat3}, {"a_color", glcore::GlBufferElement::DataType::kFloat4}, {"a_texcoord", glcore::GlBufferElement::DataType::kFloat2}});

    auto mesh = std::make_shared<glcore::Mesh>();
    mesh->Create(vertices, (uint32_t)sizeof(vertices), layout, indices, 6); //NOLINT
    return mesh;
}

std::shared_ptr<glcore::Mesh> CreateTetrahedron() {

    unsigned int indices[] = {
        0, 3, 1, //
        1, 3, 2,//
        2, 3, 0,//
        0, 1, 2 //
    };

    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,// r0
        0.0f, -1.0f, 1.0f, 0.0F, 1.0F, 0.0F, 1.0F, 0.5F, 0.0F,// r1
        1.0f, -1.0f, 0.0f, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 0.0F, // r2
        0.0F, 1.0F, 0.0F,0.0F, 0.0F, 0.0F, 1.0F, 0.5F, 1.0F
    };
    auto layout = glcore::GlBufferLayout({{"a_position", glcore::GlBufferElement::DataType::kFloat3}, {"a_color", glcore::GlBufferElement::DataType::kFloat4}, {"a_texcoord", glcore::GlBufferElement::DataType::kFloat2}});

    auto mesh = std::make_shared<glcore::Mesh>();
    mesh->Create(vertices, (uint32_t)sizeof(vertices), layout, indices, 12); //NOLINT
    return mesh;
}

int32_t main(int32_t argc, char** argv) {  //NOLINT
    GLFWwindow* window = nullptr;

    /* (GLFW) GLFW 초기화*/
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "Failed to initialize GLFW" << '\n';
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
        std::cout << "GLEW initialisation failed!"<< "\n";
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
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    
    glEnable(GL_DEPTH_TEST);  // Depth 설정, Depth에 따른 UI 그리려면 필요함.

    /* Mesh 생성 --------------------------------------------------------------*/
    // auto mesh = CreateRectangle();
    auto obj1 = CreateTetrahedron();
    auto obj2 = CreateTetrahedron();

    /* 카메라 생성 --------------------------------------------------------------*/
    glcore::PerspCamera camera(45.0f, 640.0f, 480.0f, 0.1f, 100.0f);

    /* Texture 생성 -----------------------------------------------------------*/
    auto brick_texture = glcore::Texture2D(glcore::TextureImage("brick_texture", "../gl-sandbox/assets/textures/brick.png"));

    auto dirt_texture = glcore::Texture2D(glcore::TextureImage("dirt_texture", "../gl-sandbox/assets/textures/dirt.png"));

    /* Shader 설정 및 컴파일 ----------------------------------------------------*/
    glcore::Shader shader("../gl-sandbox/assets/shaders/ambient_light.glsl");
    glcore::Light light(glm::vec3(1.0F, 1.0F, 1.0F), 0.8F, 0.0F);
    /* 랜더링 루프 --------------------------------------------------------------*/
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        double now = glfwGetTime();
        delta_time = now - last_time;
        last_time  = now;
        /* 배경색 설정 */
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // dark grey

        /* 프레임 버퍼를 배경색으로 최기화 */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* 카메라 업데이트 */
        camera.OnUpdateTmp(window, 0.0f);

        /* shader binding */
        shader.Bind();
        shader.SetMat4("u_view_projection_matrix",
                       camera.GetViewProjectionMatrix());
        shader.SetFloat3("u_light.color", light.color());
        shader.SetFloat("u_light.intensity", light.ambient_intensity());
        
        glm::mat4 model_matrix{1};
        shader.SetMat4("u_model_matrix", model_matrix);
        brick_texture.Bind();
        obj1->Render();

        model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 1.0f, 1.5f));
        shader.SetMat4("u_model_matrix", model_matrix);
        dirt_texture.Bind();
        obj2->Render();
        glcore::Shader::Unbind();
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
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
#include "gl-core/renderer/material.h"
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

    std::vector<uint32_t> indices = {
        0, 1, 2, //
        2, 3, 0
    };

    std::vector<glcore::TriangleMesh::VertexInput> vertices(4);
    vertices[0] = {{-0.5f, -0.5f, 0.0f},  {0.0F, 0.0F},{1.0F, 0.0F, 0.0F, 1.0F}};
    vertices[1] = {{0.5f, -0.5f, 1.0f}, {1.0F, 0.0F}, {0.0F, 1.0F, 0.0F, 1.0F}};
    vertices[2] = {{0.5f, 0.5f, 0.0f}, {1.0F, 1.0F}, {0.0F, 0.0F, 1.0F, 1.0F}};
    vertices[3] = {{-0.5F, 0.5F, 0.0F}, {0.0F, 1.0F},{0.0F, 0.0F, 0.0F, 1.0F}};

    auto layout = glcore::GlBufferLayout({{"a_position", glcore::GlBufferElement::DataType::kFloat3}, {"a_color", glcore::GlBufferElement::DataType::kFloat4}, {"a_texcoord", glcore::GlBufferElement::DataType::kFloat2}});

    auto mesh = std::make_shared<glcore::TriangleMesh>();
    mesh->Create(vertices, indices); //NOLINT
    return mesh;
}

std::shared_ptr<glcore::Mesh> CreateFloor() {
    /* VBO -------------------------------------------------------------------*/
    // r3 ----- r2
    // |         |
    // r0 ----- r1
    // clang-format off

    std::vector<uint32_t> indices = {
        0, 1, 2, //
        2, 3, 0
    };
   
    std::vector<glcore::TriangleMesh::VertexInput> vertices(4);
    vertices[0] = {{-10.0F, 0.0F, -10.0F}, {0.0F, 0.0F}, {0.0F, 0.0F, 0.0F, 0.0F}};
    vertices[1] = {{10.0F, 0.0F, -10.0F}, {1.0F, 0.0F}, {0.0F, 0.0F, 0.0F, 0.0F}};
    vertices[2] = {{10.0F, 0.0F, 10.0F}, {1.0F, 1.0F}, {0.0F, 0.0F, 0.0F, 0.0F}};
    vertices[3] = {{-10.0F, 0.0F, 10.0F}, {0.0F, 1.0F},{0.0F, 0.0F, 0.0F, 0.0F}};

    auto layout = glcore::GlBufferLayout({{"a_position", glcore::GlBufferElement::DataType::kFloat3}, {"a_color", glcore::GlBufferElement::DataType::kFloat4}, {"a_texcoord", glcore::GlBufferElement::DataType::kFloat2}});

    auto mesh = std::make_shared<glcore::TriangleMesh>();
    mesh->Create(vertices, indices); //NOLINT
    return mesh;
}

std::shared_ptr<glcore::Mesh> CreateTetrahedron() {

    std::vector<uint32_t> indices = {
        0, 3, 1, //
        1, 3, 2,//
        2, 3, 0,//
        0, 1, 2 //
    };

    std::vector<glcore::TriangleMesh::VertexInput> vertices(4);
    vertices[0] = {{-1.0f, -1.0f, 0.0f}, {0.0F, 0.0F}, {1.0F, 0.0F, 0.0F, 0.0F}};
    vertices[1] = {{0.0f, -1.0f, 1.0f}, {0.5F, 0.0F}, {0.0F, 1.0F, 0.0F, 0.0F}};
    vertices[2] = {{1.0f, -1.0f, 0.0f}, {1.0F, 0.0F}, {0.0F, 1.0F, 0.0F, 0.0F}};
    vertices[3] = {{0.0F, 1.0F, 0.0F}, {0.5F, 1.0F},{0.0F, 0.0F, 0.0F, 0.0F}};

    auto layout = glcore::GlBufferLayout({{"a_position", glcore::GlBufferElement::DataType::kFloat3}, {"a_color", glcore::GlBufferElement::DataType::kFloat4}, {"a_texcoord", glcore::GlBufferElement::DataType::kFloat2}});

    auto mesh = std::make_shared<glcore::TriangleMesh>();
    mesh->Create(vertices, indices); //NOLINT
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
    auto floor = CreateFloor();

    /* 카메라 생성 --------------------------------------------------------------*/
    glcore::PerspCamera camera(45.0f, 640.0f, 480.0f, 0.1f, 100.0f);

    /* Texture 생성 -----------------------------------------------------------*/
    auto brick_texture = glcore::Texture2D(glcore::TextureImage("brick_texture", "../gl-sandbox/assets/textures/brick.png"));

    auto dirt_texture = glcore::Texture2D(glcore::TextureImage("dirt_texture", "../gl-sandbox/assets/textures/dirt.png"));
    
    auto plain_texture = glcore::Texture2D(glcore::TextureImage("plain_texture", "../gl-sandbox/assets/textures/plain.png"));



    /* Shader 설정 및 컴파일 ----------------------------------------------------*/
    glcore::Shader shader("../gl-sandbox/assets/shaders/phong_light.glsl");
    
    glcore::DirectionalLight dlight(glm::vec3(1.0F, 1.0F, 1.0F), 0.1, 0.1F,glm::vec3(0.0F, 0.0F, -1.0F));


    glcore::PointLight plight1(glm::vec3(0.0F, 0.0F, 1.0F), 0.0F, 1.0F, glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.3F, 0.2F, 0.1F));

    glcore::PointLight plight2(glm::vec3(0.0F, 1.0F, 0.0F), 0.0F, 1.0F, glm::vec3(-4.0F, 2.0F, 0.0F), glm::vec3(0.3F, 0.1F, 0.1F));

    // glcore::SpotLight slight1(glm::vec3(1.0F, 0.0F, 0.0F), 0.0F, 2.0F, glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, -1.0F, 0.0F), glm::vec3(1.0F, 0.0F, 0.0F), 20.0F);
    glcore::SpotLight slight1(glm::vec3(1.0F, 1.0F, 1.0F), 0.3F, 1.0F, glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.1F, 0.2F, 0.01F), glm::vec3(1.0F, 0.0F, 0.0F), 20.0F);

    glcore::SpotLight slight2(glm::vec3(1.0F, 1.0F, 1.0F), 0.0F, 1.0F, glm::vec3(0.0F, -1.5F, 0.0F), glm::vec3(-100.0F, -1.0F, 0.0F), glm::vec3(1.0F, 0.0F, 0.0F), 20.0F);

    glcore::Material shinyMaterial = glcore::Material(1.0F, 32.0F);
    glcore::Material dullMaterial = glcore::Material(0.3F, 4.0F);
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
        slight1.SetFlash(camera.GetPosition() - glm::vec3(0.0F, 0.5F, 0.0F), camera.GetDirection());

        /* shader binding */
        shader.Bind();
        shader.SetMat4("u_view_projection_matrix",
                       camera.GetViewProjectionMatrix());
        shader.SetFloat3("u_eye_position", camera.GetPosition());
        dlight.UseLight(shader);
        plight1.UseLight(shader);
        plight2.UseLight(shader);
        slight1.UseLight(shader);
        slight2.UseLight(shader);

        //* Object 1
        glm::mat4 model_matrix{1.0F};
        model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, -2.5f));
        model_matrix = glm::scale(model_matrix, glm::vec3(0.6f, 0.6f, 1.0f));
        shader.SetMat4("u_model_matrix", model_matrix);
        shinyMaterial.UseMaterial(shader);
        brick_texture.Bind();
        // plain_texture.Bind();
        obj1->Render();
        
        //* Object 2
        model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 1.0f, -2.5f));
        model_matrix = glm::scale(model_matrix, glm::vec3(0.6f, 0.6f, 1.0f));
        shader.SetMat4("u_model_matrix", model_matrix);
        dullMaterial.UseMaterial(shader);
        dirt_texture.Bind();
        obj2->Render();

        //* Floor
        model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, -2.0f, -0.0f));
        shader.SetMat4("u_model_matrix", model_matrix);
        shinyMaterial.UseMaterial(shader);
        dirt_texture.Bind();
        floor->Render();
        glcore::Shader::Unbind();
        glcore::Texture::Unbind();
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
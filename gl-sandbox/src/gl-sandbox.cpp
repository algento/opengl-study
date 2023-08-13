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

#include "glpch.h"

unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result = 0;
    //* i: integer, v: vector
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length = 0;
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

unsigned int CreateShader(const std::string& vertexShader,
                          const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs      = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs      = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

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

int main(int argc, char** argv) {  //NOLINT
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
    // int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // if (status == 0) {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }

    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    /* VBO -------------------------------------------------------------------*/
    GLuint vbo = 0;         // vbo 버퍼 핸들 생성
    glGenBuffers(1, &vbo);  // 핸들을 입력받아 1개의 버퍼를 만든다.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // vbo 핸들에 타입을 바인딩한다.

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // left
        0.0f,  0.5f,  0.0f,  // top
        0.5f,  -0.5f, 0.0f   // right
    };

    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), &vertices[0],
                 GL_STATIC_DRAW);  // 바인딩딘 버퍼에 데이터를 복사한다.

    /* VAO -------------------------------------------------------------------*/
    GLuint vao = 0;              // vao 핸들 생성
    glGenVertexArrays(1, &vao);  // 핸들을 입력받아 1개의 vao를 만든다.
    glBindVertexArray(vao);  // vao를 바인딩해서 current VAO로 만든다.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // 해당 VAO와 VBO를 수정하지 않도록 0으로 바인딩한다.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* Shader 설정 및 컴파일 ----------------------------------------------------*/
    std::string vertexShader =
        R"(#version 330 core
        layout(location = 0) in vec4 position;
        void main() {
           gl_Position = position;
        })";
    std::string fragmentShader =
        R"(#version 330 core
        layout(location = 0) out vec4 color;
        void main() {
           color = vec4(1.0, 0.0, 0.0, 1.0);
        })";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* 랜더링 루프 --------------------------------------------------------------*/
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        /* 배경색 설정 */
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // dark grey

        /* 프레임 버퍼를 배경색으로 최기화 */
        glClear(GL_COLOR_BUFFER_BIT);

        /* OpenGL이 해당 vao를 사용하도록, vao를 바인딩 */
        glBindVertexArray(vao);

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
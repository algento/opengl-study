# Shader

- OpenGL 랜더링 파이프라인은 고정 함수 블록과 프로그래밍 가능한 쉐이더 블록을 포함한 여러 개의 스테이지로 이루어져 있다. Shader는 GPU에서 수행하는 프로그램이며, OpenGL은 이를 고정함수로 연결시켜 작동한다. GPU는 단순한 연산을 수행하는데 유리하며, 이를 대규모의 core를 통해 병렬처리함으로써 애플리케이션 수행 속도를 향상시킬 수 있다. OpenGL은 스크린의 각 픽셀에 어떤 색을 출력할지를 연산하기 위해 GPU를 사용하며 이 때 GPU에서 Shader를 병렬로 수행한다. 이를 통해 빠르게 화면에 출력한 이미지를 생성할 수 있다.
- OpenGL 쉐이더는 openGL Shading Language (GLSL)로 작성한다. C와 유사하지만 완벽하게 동일하지는 않고 그래픽스 프로세서에서 동작할 수 있는 특수한 기능들을 지원한다. 이 언어의 컴파일러는 OpenGL에 내장되어 있고, 쉐이더 소스를 작성하여 OpenGL에 전달하면 이는 **쉐이더 객체**로 바뀌어 컴파일되고 여러 쉐이더 객체들이 하나의 **프로그램 객체**로 링크된다. 간단한 그래픽을 처리하려면 vertex shader와 fragment shader만 사용해도 충분하다. 추가로 처리를 원할 경우, tessellation control/evaluation shader, geometry shader, compute shader를 추가로 사용할 수 있다. 게임 엔진 같은 경우는 라이브로 shader 스크립트를 생성하고 이를 GPU에 전달하여 내가 원하는 그래픽을 출력하기도 한다.
- OpenGL 쉐이더도 state machine으로 동작하기 때문에 특정 순서로 호출하지 않으면 제대로 동작하지 않는다. 쉐이더를 생성하고 수행하는 과정은 다음과 같다.
  - `glCreateShader()`: 빈 쉐이더 객체를 생성한다.
  - `glShaderSource()`: 쉐이더 소스 코드를 쉐이더 객체로 전달(복사)한다.
  - `glCompileShader()`: 쉐이더 객체에 포함된 소스코드를 컴파일한다.
  - `glCreateProgram()`: 쉐이더 객체에 attach시킬 프로그램 객체를 생성한다.
  - `glAttachShader()`: 프로그램 객체에 쉐이더 객체를 attach한다.
  - `glLinkProgram()`: 프로그램에 attach된 모든 쉐이더 객체를 링크한다.
  - `glDeleteShader()`: 쉐이더 객체를 삭제한다. 쉐이더가 프로그램 객체이 링크되면 바이너리가 보관되기 때문에 쉐이더 객체는 더이상 필요하지 않다.
  - `glUsePorgram()`: 랜더링을 위해 프로그램 객체를 사용한다.
  - `glDeleteProgram()`: 사용을 마친 프로그램 객체를 삭제한다.

```c++
GLuint glCreateProgram(void);
// (https://docs.gl/gl4/glCreateProgram)
// 오류 발생 시 0을 반환한다.

GLuint glCreateShader(GLenum shaderType);
// (https://docs.gl/gl4/glCreateShader)
// - shaderType: 프로그램 가능한 프로세서의 타입을 지정한다.
//   - GL_COMPUTE_SHADER: compute 프로세서
//   - GL_VERTEX_SHADER: vertex 프로세서
//   - GL_TESS_CONTROL_SHADER: tessellation 프로세서
//   - GL_TESS_EVALUATION_SHADER: tessellation 프로세서의 evaluation 단계
//   - GL_GEOMETRY_SHADER: geometry 프로세서
//   - GL_FRAGMENT_SHADER: fragment 프로세서
// 오류 발생 시 0을 반환한다.

void glShaderSource(GLuint shader, GLsizei count, 
                    const GLchar **string, const GLint *length);
// (https://docs.gl/gl4/glShaderSource)
// - shader: 쉐이더 객체의 핸들
// - count: 전달되는 소스 코드의 개수
// - string: 소스 코드의 배열 (여러 개의 소소 코드가 들어갈 수 있으므로)
// - length: 소스 코드 길이의 배열, NULL이 입력되면, null terminated로 가정

void glCompileShader(GLuint shader);
// (https://docs.gl/gl4/glCompileShader)
// - shader: 쉐이더 객체의 핸들

void glAttachShader(GLuint program, GLuint shader);
// (https://docs.gl/gl4/glAttachShader)
// - program: 프로그램 객체의 핸들
// - shader: 쉐이더 객체의 핸들

void glLinkProgram(GLuint program);
// (https://docs.gl/gl4/glLinkProgram)
// - program: 프로그램 객체의 핸들

void glValidataProgram(GLuint program);
// (https://docs.gl/gl4/glValidataProgram)
// 프로그램 객체의 유효성을 검증한다. 해당 프로그램의 바이너리를 OpenGL이 실핼할 수 있는지를 검증한다.
// - program: 프로그램 객체의 핸들

void glDeleteShader(GLuint shader);
// (https://docs.gl/gl4/glDeleteShader)
// 쉐이더 객체를 메모리 해제하면서 전달된 핸들도 제거한다. 예를 들어 메모리에 등록된 쉐이더 객체 ID가 1, 2, 3이 있을 때 1을 제거할 경우, 다음에 생성한 쉐이더 객체 ID는 1로 할당
// - shader: 쉐이더 객체의 핸들

void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
// (https://docs.gl/gl4/glGetShaderInfoLog)
// - shader: 쉐이더 객체의 핸들
// - maxLength: 로그를 저장할 문자열 버퍼의 최대 크기
// - length: infoLog에 전달받은 문자열 버퍼의 크기를 저장할 int32_t 변수 포인터
// - infoLog: OpenGL에서 전달받은 문자열 배열의 시작 주소

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
// (https://docs.gl/gl4/glGetShaderiv)
// 지정한 쉐이더의 정수형 정보를 가져온다.
// - shader: 쉐이더 객체의 핸들
// - pname: 객체 파라미터 지정
//  - GL_SHADER_TYPE: 쉐이더 타입을 반환
//  - GL_DELETE_STATUS: 삭제 플래그가 설정되어 있으면, true를 반환
//  - GL_COMPILE_STATUS: 컴파일이 잘 되었으면, true를 반환
//  - GL_INFO_LOG_LENGTH: INFO_LOG에 있는 글자수를 반환
//    - 해당 길이만크 메모리를 할당하고 glGetShaderInfoLog를 이용해서 로그 정보를 가져올 수 있다.
//  - GL_SHADER_SOURCE_LENGTH: 소스코드 길이를 반환
// - params: 파라미터를 받아올 int32_t 변수 포인터

void glUseProgram(GLuint program);
// (https://docs.gl/gl4/glUseProgram)
// - program: 프로그램 객체의 핸들

void glDeleteProgram(GLuint program);
// (https://docs.gl/gl4/glDeleteProgram)
// - program: 프로그램 객체의 핸들

```

## GLSL Language

// TODO

## Vertex Shader

- Vertex shader는 vertex마다 호출되며, 기본적으로 vertex가 어디에 위치하는지 등의 속성을 결정한다. Vertex shader는 VAO에서 전달된 정점 데이터를 입력받아 클립 공간에서의 정점 데이터를 출력한다.
- Vertex shader가 가지는 built-in 변수는 다음과 같다.
  - `gl_Position`
  - `gl_PoitnSize`
  - `gl_VertexID`

```c
#version 330 core //Core 프로파일 버전 지정 (여기서는 3.30)

layout(location = 0) in vec4 position; // 정점 position attribute의 인덱스를 location에 전달

void main() {
    // glsl의 내장 변수인 gl_Postion을 통해서 클립 공간에서의 정점의 위치를 지정한다.
    gl_Position = position;
}
```

## Fragment Shader

- fragment shader는 rasterizer가 만들어준 프래그먼트 데이터(랜더링할 픽셀)을 입력받아 픽셀 별로 동작한다. 즉, 색깔을 그려야할 픽셀이 정해지면 fragment shader도 그 수만큼 동작해서 각 픽셀의 색을 계산한다.
- Fragment shader가 가지는 built-in 변수는 다음과 같다.
  - `gl_FragCoord`
  - `gl_FontFacing`
  - `gl_FragDepth`

```c
#version 330 core

layout(location = 0) out vec4 color; // Fragment shader가 출력할 컬러 값 (일반적으로 location을 0으로 지정한다.)

void main() {
    color = vec4(1.0, 0.0, 0.0, 1.0); // red
}
```

## Reference

1. [LearnOpenGL-Advanced GLSL](https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL)

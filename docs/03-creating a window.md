# Creating a Window in C++

- 운영체제에 상관없이 OpenGL context를 시작하는 것을 도와주는 helper library들이 있다. 즉, 운영체제에 상관없이 동일한 인터페이스로 Window를 만드는 것은 도와준다.(실제 윈도우는 운영체제에서 알아서 만든다.) GLFW는 이러한 라이브러리 중 SDL과 더불어 가장 유명하다.

## GLFW

- 바이너리를 받아서 사용하면 된다. 하지만 개인 프로젝트를 한다면 코드 디버깅에 도움이 될 수 있으므로 GLFW 소스코드를 받아서 빌드해서 사용해도 된다.
- 바이너리는 64/32 비트가 제공된다.
- MS Visual Studio 사용 시 유의점
  - Relative path에 기반해서 솔루션을 설정하는 것이 좋으며, 이 때 VS에서 제공하는 매크로를 쓰면 유용하다.
    - SolutionDir: 솔루션 파일이 있는 디렉토리
    - Linker/Input에서 glfw3 라이브러리 파일을 추가해야 한다.
  - glfw3.lib이외에 OpenGL32.lib, User32.lib, Gdi32.lib, Shell32.lib을 추가해야한다.
- 맥은 OpenGL 최신 버전을 지원하지 않으며 그마저도 불완전하게 지원한다. 현재 지원하는 버전은 4.1까지 이며 다른 운영체제는 최신 버전의 (4.5 이상) OpenGL을 사용할 수 있다.

```c++
// GLFW 초기화
if (glfwInit() == GLFW_FALSE) {
    return -1;
}

// GLFW 윈도우 힌트 설정 (윈도우를 어떻게 만들지에 대해 설정할 수 있다.)
glfwWindowHint(GLFW_SAMPLES, 4);  // 4x Antialiasing
#if defined(PLATFORM_WINDOWS)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPACT_PROFILE);
#elif defined(PLATFORM_UNIX)
#elif defined(PLATFORM_APPLE)
    // Apple Mac에서 OpenGL 4.1 버전을 사용하기 위한 설정
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                  GL_TRUE);  // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

// GLFW 윈도우 생성
window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
if (window == nullptr) {
    glfwTerminate();
    return -1;
}

// 생성한 윈도위의 glfw context 생성
glfwMakeContextCurrent(window);
```

## GLAD

- OpenGL은 실제로 인터페이스일 뿐이므로 특정 그래픽 카드가 지원하는 드라이버에 사양을 구현하는 것은 드라이버 제조업체에 달려있다. 또한 OpenGL 드라이버에 여러 가지 다른 버전들이 있으므로 함수 대부분의 위치는 컴파일 타임(compile-time)에 알 수 없고 런타임(run-time)에 필요한 함수의 위치를 검색하고 이후에 사용을 위해 함수 포인터로 함수를 저장해두어야 한다.함수의 위치를 검색하는 것은 운영체제마다 다르며 (OS-specific), 윈도우는 다음과 같은 과정을 거친다.

```c++
// define the function's prototype
typedef void (*GL_GENBUFFERS) (GLsizei, GLuint*);
// find the function and assign it to a function pointer
GL_GENBUFFERS glGenBuffers  = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
// function can now be called as normal
uint32_t buffer;
glGenBuffers(1, &buffer);

```

- GLAD는 운영체제와 그래픽 드라이버에 따라 OpenGL 함수를 검색하는 번거로운 작업들을 관리해준다.
- [GLAD 홈페이지](https://glad.dav1d.de)로 이동하여 언어 (여기서는 C++), 버전에 따라 직접 다은로드해야한다.
- 프로파일은 Core로 설정하고 `Generarte a loader` 옵션이 체크되어 있는지 확안한다.

```c++
...
// 생성한 윈도위의 glfw context 생성
glfwMakeContextCurrent(window);

// GLAD 로딩 (내 하드웨어에 맞는 OpenGL)
gladLoadGL();
/* GLFW에 맞는 GLAD를 로딩한다.
int32_t status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
if (status == 0) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
} 
*/
```

## GLEW

- GLAD보다도 많이 사용되는 Wrangler 라이브러리인 GLEW는 pre-build된 라이브러리를 사용하거나 직접 소스를 다운받아서 빌드해서 사용해야 한다. 주의할 점은 github에서 다운받아서 빌드하면 include 헤더가 없기 때문에 빌드가 되지 않으니 github clone이 아니라 github page에서 제공하는 zip 파일을 다운받아서 빌드해야 한다.
- glewGetErrorString()으로 error를 확인할 수 있다.
- 특정 extension이 있는지 확인할 수 있다.
  
```c++
...
// Allow modern extension features
glewExperimental = GL_TRUE;
if (glewInit() != GLEW_OK) {
    std::cout << "GLEW initialisation failed!"
              << "\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
}
if (!GLEW_EXT_framebuffer_object){
  ...
}
```

## Rendering Window

- glViewport(..)
  - 스크린 좌표로 변환?
  - 창의 크기가 변하면, 뷰포트도 조정해야 함.
  - 뷰포트 변화에 따라 프레임버퍼를 변경할 수 있도록 콜백 설정
    - 콜백 함수
      - void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    - 콜백 설정
      - glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
- glfwWindowShouldClose(window)
  - 윈도우 종료 명령이 내려졌는지 확인
- glfwSwapBuffers(window)
  - double buffer 랜더링 사용
    - back buffer: 화면에 출력하는 데이터
    - front buffer: 화면 출력 전에 미리 계산된 출력 데이터
- glfwPollEvent()
  - 이벤트 발생했는지 확인하고 윈도우 상태를 업데이트하고 콜백함수 호출

## Terminating Window

- glfwTerminate()
  - 윈도우 객체가 생성되지 않을 경우, 호출하여 GLFW 종료

## Reference

1. [Anton Gerdealn - "Hello Triangle"](https://antongerdelan.net/opengl/hellotriangle.html)
2. [Anto Gerdelan - "Extended Initialisation"](https://antongerdelan.net/opengl/glcontext2.html)

# Creating a Window in C++

- 운영체제에 상관없이 OpenGL context를 시작하는 것을 도와주는 helper library들이 있다. 즉, 운영체제에 상관없이 동일한 인터페이스로 Window를 만드는 것은 도와준다.(실제 윈도우는 운영체제에서 알아서 만든다.) GLFW는 이러한 라이브러리 중 SDL과 더불어 가장 유명하다.
- GLFW 사용법
  - 바이너리를 받아서 사용하면 된다. 하지만 개인 프로젝트를 한다면 코드 디버깅에 도움이 될 수 있으므로 GLFW 소스코드를 받아서 빌드해서 사용해도 된다.
  - 바이너리는 64/32 비트가 제공된다.
  - MS Visual Studio 사용 시 유의점
    - Relative path에 기반해서 솔루션을 설정하는 것이 좋으며, 이 때 VS에서 제공하는 매크로를 쓰면 유용하다.
      - SolutionDir: 솔루션 파일이 있는 디렉토리
      - Linker/Input에서 glfw3 라이브러리 파일을 추가해야 한다.
    - glfw3.lib이외에 OpenGL32.lib, User32.lib, Gdi32.lib, Shell32.lib을 추가해야한다.
- 맥은 OpenGL 최신 버전을 지원하지 않으며 그마저도 불완전하게 지원한다. 현재 지원하는 버전은 4.1까지 이며 다른 운영체제는 최신 버전의 (4.5 이상) OpenGL을 사용할 수 있다.

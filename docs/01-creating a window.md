# Creating a Window in C++

- Window를 만드는 것은 플랫폼이 알아서 한다.
- 이것을 도와주는 라이브러리가 GLFW이다. GLFW의 장점은 가볍고 윈도우를 생성하고 OpenGL context 등을 쉽게 다룰 수 있게 한다. SDL과 차이점은?
- GLFW 소스코드를 받아서 빌드해서 사용해도 된다. 개인 프로젝트를 한다면 코드 디버깅에 도움이 될 수 있다.
  - 32/64 비트 바이너리가 있다.
  - 64 비트 코드는 32비트 운영체제에서는 돌아가지 않지만, 64 비트 운영체제에서는 둘 다 돌릴 수 있다.
- Windows에서 사용할 경우, 라이브러리와 인클루드 파일을 직접 설정해줘야 한다. (불편함)
- Relative path에 기반해서 솔루션을 설정하는 것이 좋으며, 이 때 VS에서 제공하는 매크로를 쓰면 유용하다.
  - SolutionDir: 솔루션 파일이 있는 디렉토리
  - Linker/Input에서 glfw3 라이브러리 파일을 추가해야 한다.
- VS의 Error list보다는 컴파일러 output을 보는 것을 추천한다.
- 윈도우에서 glfw3.lib이외에 OpenGL32.lib, User32.lib, Gdi32.lib, Shell32.lib을 추가해야한다.

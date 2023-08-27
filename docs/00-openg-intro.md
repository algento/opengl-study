# Introduction to OpenGL

## Definition

- OpenGL은 애플리케이션이 그 하부에서 동작하는 장치의 그래픽스 서브시스템 (e.g. 그래픽 카드)에 접근하고 제어하는데 필요한 표준화된 인터페이스 (추상화 레이어)이다. 프로그램에서 만들어진 명령들이 OpenGL을 할당되며, 할당된 명령은 파이프라인 분할과 병렬화를 통해 그래픽스 하드웨어로 보내진다. 개발자가 운영체제나 그래픽스 하드웨어에 신경쓸 필요없이 공통의 인터페이스를 통해 그래픽스 시스템을 제어할 수 있기 때문에 cross-platform을 지원할 수 있다.
- OpenGL은 단순히 표준 요구사항일 뿐이며 각 그래픽카드 제조사가 드라이버를 만들 때, 해당 구현을 포함하여 배포한다. 따라서 각 운영체제나 그래픽 카드 제조사의 지원 여부에 따라 사용에 제한이 있을 수 있다. 또한 OpenGL은 적절한 수준의 추상화 수준으로 구현되어 있기 때문에 단순한 프로그램을 구현하는 것이 훨씬 쉬우며, 그에 따라 API도 상대적으로 다른 것들에 비해 작고 단순하지만 고급 그래픽스 기능을 사용할 수 없다. 따라서 운영체제에서 지원하는 자체 graphic API인 direct3D (Windows), Metal (MacOS)보다 사용하기 쉽지만 더 낮은 성능을 보인다.
- Vulcan은 OpenGL과 동일한 cross-platform API이며, 좀 더 low-level의 기능까지 다룰 수 있다. 하지만 사용자가 적은 편이기 때문에 예제 코드, 문서 등을 웹에서 구하기 어렵다.
- OpenGL은 거대한 state machine으로 구현되며 이를 OpenGL context라 한다. 일반적으로 stage-setting (bind하기) -> stage-using의 단계를 거친다.

## History

- OpenGL 1.1버전은 1997에 배포되었고, 2.1 버전은 2006년에 3.1 버전은 2009년, 4.1 버전은 2010년에 각각 배 포 되었다. 보통 버전 3 이상을 modern OpenGL이라고 한다.
- Modern OpenGL에서는 현대 그래픽스 하드웨어로 가속성능을 이용하는 core profile과 구버전을 지원하기 위한 Compatability profile로 구분되었고 호환성 프로파일은 1.0부터 현재까지의 모든 버전과의 호환성을 지원한다.
- 일반적으로 core profile만을 다루므로 호환성 프로파일을 사용하는 경우는 별도로 공부를 해야한다. core-profile의 경우 사양서를 기반으로 현대적으로 재작성했기 때문에 어렵지만 효율적이며, 구버전을 지원하는 immediate mode를 사용하는 경우 사용 및 이해가 쉽지만 비효율적이다.
- OpenGL은 새로운 기술이 등장할 때마다 이를 활용할 수 있도록 확장을 제공한다.
- Modern OpenGL이 구 OpenGL과 가지는 가장 큰 차이점은 shader이다. 기존의 OpenGL이 가지고 있던 비효율적인 랜더링 지원 함수를 삭제하고 shader를 이용해서 효율높은 랜더링을 수행하는 것이 가능하다.

## OpenGL Wrangler

- OpenGL 기능과 연관된 function은 Graphic 카드 드라이버에 구현되어 있다. 하지만 이것을 직접 불러와 실행시키려면, 운영체제와 그래픽 카드제조사의 사양에 영향을 받게 된다. 따라서 이런 Graphic 기능을 공통을 인터페이스로 손쉽게 사용할 수 있도록 포장한 것이 OpenGL Extension Wrangler이다.
- OpenGL Extention Wrangler는 OpenGL에 대한 interface와 platform specific한 OpenGL extension 의 loadingdmf 지원한다. OpenGL은 사양이기 때문에 그래픽카드 구현과 platform specific한 extension을 일일이 확인하여 사용하는 것은 매우 고된 일이다. 따라서 wrangler가 자동으로 이런 것들을 처리해주기 때문에 매우 유용하기 때문에 기본적으로 사용된다. 대표적인 라이브러리로 glew (OpenGL Extention Wrangler Library)나 glad, SDL e둥이 있으며 glew가 가장 많이 사용되었지만 최근에는 glad도 많이 사용되고 있다.

### GLEW

### GLAD

## OpenGL Framework

- OpenGL을 이용하여 platform에 맞게 윈도우를 생성하고 제어하는 것을 돕는 기능을 하는 framework들이 존재한다. 윈도우를 생성하며 해당 윈도우에 대한 OpenGL context를 제공하며, 다양한 기능들 (키보드/마우스/패드 입력, 오디오, 다중 모니터 지원 등)을 지원한다.
- 대표적인 OpenGL Framework 라이브러리로는 glfw와 SDL이 있으며, SDL이 더 무거운 대신 더 많은 기능들 (audio supprot, threading, filesystem, etc.)을 지원한다. 이런 이유때문에 SDL은 인디게임이나 인디 게임엔지 개발에 많이 사용되었다. (CryEngine, SourceEngine, Amnesia, Dying Light, etc.)

## GPU

- OpenGL의 그래픽 프레세싱 유닛은 GPU에 접근할 수 있는 기능을 제공하며, GPU은 매우 유연해서 그래픽스와 관련없는 물리 시뮬레이션, 인공 지능 등에도 사용되고 있다.
- GPU는 Shader로 불리는 작은 프로그램을 실행하는 Shader Core로 이루어져 있으며, 각 코어는 단순하고 낮은 처리량을 가지지만 병렬적으로 처리할 수 있기 때문에 속도를 향상시킬 수 있다.
- Shader 소스를 OpenGL을 통해서 그래픽스 서브 시스템에 제공하면 그래픽스 서브시스템은 이를 컴파일하여 프로그램을 만들고 실행한다.

## Reference

1. OpenGL Super Bible
2. <https://rito15.github.io/categories/opengl-study/>

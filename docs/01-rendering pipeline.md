# Rendering Pipelin

## Premitives

- OpenGL 랜더링의 최소 단위는 premitive이다. 많은 타입의 프리미티브를 지원하지만 기본적인 것들이 점 (point), 선 (line), 삼각형 (triangle)이며 각 premitive들은 정점 (vertex)의 집합이다.
- 삼각형은 항상 convex하므로 그리는 규칙은 만들고 구현하기 쉽다. 따라서 애플리케이션은 화면에 그려질 surface를 수많은 삼각형으로 분할하고 OpenGL을 통해 래스터라이저 (rasterizer)라는 하드웨어 가속기로 보내서 랜더링을 한다.
- 래스터라이저는 3차원으로 표현된 삼각형들을 화면에 그려질 픽셀들로 변환하는 역할을 한다.

## Graphic Pipeline

![랜더링 파이프라인](figs/rendering-pipeline.png)

- 그래픽스 시스템의 랜더링 파이프라인은 두 개의 주요 파트로 구분할 수 있다. front-end는 랜더링할 surface를 프리미티브들로 분할하고 이를 래스터라이저로 보낸다. back-end는 geometry가 처리되어 각각이 픽셀화된다.
- OpenGL에서 관여하는 랜더링 파이프라인은 다음과 같이 3개의 스테이지로 구성된다.
  - Vertex Shader
  - (Optioinal) Tessellation Control Shader/Tessellation Evaluation Shader
  - Geometry Shader
  - Fragment Shader

### Vertex Shader

- 입력: VERTEX Data[]
  - VERTEX는 OpenGL에서 저장하고 있는 Geometry의 point에 대한 정보이며 position, normal, color, texture 등의 다양한 attribute에 대한 정보를 가지고 있다. 정점마다 버텍스 패칭 (혹은 풀링)이 실행되어 자동적으로 계산된다.
- 정점마다 호출되며, 정점들을 MVP 변환을 통해 Object space에서 Clip Space까지 변환한다.

- Shape Assembly
  - 정점들을 프리미티브 (점, 선, 삼각형)으로 구성한다.

### Geomtery Shader

- 입력: PREMITIVE Data[]
- 프리미티브마다 한번 씩 수행하며 프리미티브를 없애거나 추가하거나 다른 프리미티브로 변환하기도 한다.
- 변환된 프리미티브 목록은 Rasterization을 통해 픽셀과 매칭된 Fragment list를 만든다. 이 과정에서 view-frustrum 밖의 Fragment는 폐기한다.

### Fragment Shader

- 입력: FRAGMENT Data[]
- Fragment list를 입력받아 화면에 출력할 모든 픽셀의 색상 (color)을 계산하며, 이 과정에서 OpenGL의 고급 효과들이 계산되어 추가된다. 이렇게 결정된 색상은 Frame buffer로 보내져 최종 화면에 출력된다.
- Alpha test & blending에서는 깊이 (depth)와 스텐실 (stensi)을 체크한다.

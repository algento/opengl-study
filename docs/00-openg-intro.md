# Introduction to OpenGL

- OpenGL은 단순히 표준 요구사항일 뿐이다. 따라서 Open Source가 아니며, 그래픽카드 제조사가 만들어 배포한다. 그저 각 그래픽 카드 제조사가 만든 드라이버에서 공통의 인터페이스를 지원하는 것에 불과하기 때문에 운영체제나 그래픽카드에 따라 사용에 제한이 있을 수 있다. 하지만 공통의 인터페이스를 가지므로 cross-platform을 지원할 수 있다는 것이 큰 장점이다.
- OpenGL은 일반적으로 운영체제에서 지원하는 direct3D (Windows), Metal (MacOS)보다 더 낮은 성능을 보인다.하지만 OpenGL은 간단한 인터페이스만을 지원하기 때문에 단순한 프로그램을 구현하는 것이 훨씬 쉬우며, 그에 따라 API도 상대적으로 다른 것들에 비해 작고 가볍다.
- Vulcan은 OpenGL과 동일한 cross-platform API이며, 좀 더 low-level의 기능까지 다룰 수 있다. 하지만 사용자가 적은 편이기 때문에 예제 코드, 문서 등을 웹에서 구하기 어렵다.
- Modern OpenGL이 기존의 OpenGL과 가지는 가장 큰 차이점은 shader이다. 기존의 OpenGL이 가지고 있던 비효율적인 랜더링 지원 함수를 삭제하고 shader를 이용해서 효율높은 랜더링을 수행하는 것이 가능하다.

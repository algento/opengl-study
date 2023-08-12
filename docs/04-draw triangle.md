# Drawing Triangle

## Buffer

- OpenGL에서 버퍼는 여로 용도로 사용할 있는 연속적인 메모리 공간이며, **이름**으로 구별한다. 버퍼를 사용하기 전에는 OpenGL을 통해 사용할 이름을 예약하고 그 이름을 사용해서 메모리(data store)를 할당한 후 거기에 데이터를 저장해야 한다.
- OpenGL은 버퍼의 이름을 OpenGL context에 attach하고 애플리케이션을 통해 해당 버퍼에 데이터를 쓰거나 읽어올 수 있다. 이 때, attach하기 위해서는 해당 이름을 버퍼 바인딩 포인트에 bind해야 한다. OpenGL에는 다양한 버퍼 바인딩이 존재하는데 버퍼의 내용이 Vertex Shader로 자동으로 공급되거나 Shader가 사용한 변수의 값을 입력하거나 아니면 쉐이더가 생성하는 데이터를 저장할 공간으로 사용할 수 있다.

## Vertex Buffer Object (VBO)

- Vertex buffer는 정점들에 대한 정보를 가지고 있는 메모리 버퍼이며 Object 단위로 관리된다. OpenGL은 다른 그래픽스 API와 달리 명시적으로 해당 타입를 제공하지 않지만 정점의 속성을 저장하는 버퍼 바인딩 포인트(`GL_ARRAY_BUFFER`)를 제공한다.이를 통해서 다른 그래픽스 API에서 VBO라고 부르는 객체를 사용할 수 있다.
- CPU에서 많은 양의 정점 속성들을 GPU 메모리 상에 빠르게 전송하여 랜더링 효율을 높일 수 있다.

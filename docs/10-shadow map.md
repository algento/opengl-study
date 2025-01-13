# Shadow Map

**Shadow map**은 그래픽스에서 그림자를 만들어서 처리하는 효율적인 방법으로 개발되었다.
Shadow map은 기본적으로 빛을 적용할지 말지를 결정하는 마스크로 이해할 수 있으며, OpenGL 내부에서는 이를 2D 텍스처로 저장하여 활용한다.

그림자 맵은 다음의 과정으로 생성된다.

1. 그림자 맵을 **Framebuffer**를 이용하여 캡쳐한다.
1. 해당 Framebuffer를 2D texture로 쓴다.
1. 해당 2D texture 정보를 이용하여 그림자를 적용한 컬러를 랜더링한다.

따라서 그림자 매핑을 위해서는 **적어도 2번의 랜더링**이 필요하다.

1. 맵 생성 랜더링
    - 광원 관점 (light source perspective)에서 Scene 상의 객체에 대한 depth map을 생성한다.
        - 컬러 정보를 만들 필요없다. 광원 입장에서 가려져있는지 아닌지만 판단하면 된다.
        - **Depth Test**를 통해 생성한 Depth buffer 정보만 있으면 된다.
        - Depth buffer는 frustum 상에서 깊이 방향 위치를 [0, 1]의 값으로 제공한다.
            - 0: Near plane (카메라 가까운 면)
            - 1: Far plane (카메라와 먼 면)
    - 기본 framebuffer가 아닌 별도의 framebuffer를 이용해서 depth map을 저장한다.
    - 별도의 fragment 쉐이더는 필요없다. 어차피 depth 값만 사용할 것이다.
    - **Directional light*을 위해서는 Orthographic projection이 필요하다.
1. 맵 사용 랜더링
    - 해당 depth map이 저장된 2D 텍스처를 로딩하여 Scene의 컬러를 결정한다.
    - Depth map에 기반하여 그림자를 어떻게 적용할지 결정하기 위해 현재의 fragment position이 광원 입장에서 어느 위치에 있는지 알아야 한다.
        - 해당 위치 정보를 이용해서 2D 텍스처로 저장된 depth map에서 depth 값을 추출할 수 있다.
        - 해당 위치를 **NDC(Normalized Device Coordinate**) 상의 값으로 계산해야 한다.
        - 이를 위해 쉐이더에 해당 정보를 유니폼으로 전달해야 한다.
    - Naive 방법:
        - Depth map에서 광원 입장에서의 depth value를 **closest**로 추출하고, 현재 랜더링하는 픽셀의 광원 입장에서의 depth 값을 비교하여 그림자를 바이너리로 적용한다.

Naive하게 depth map을 이용해서 그림자를 랜더링하면 다음과 같은 이슈가 발생한다.

1. **Shadow Acne**
    - 그림자의 경계가 현실과 달리 너무 확연하고, 해상도로 인해 발생하는 이산 현상(discretization)으로 Shadow Acne가 생긴다
    - 개선 방안: 작은 바이어스 추가
        - 각 물체를 살짝 카메라 쪽으로 이동시키면 **Shadow Acne**을 극복할 수 있다.
        - 다만, 이로 인해 물체 아래쪽 그림자가 사라지는 **Peter Panning**이 발생한다.
1. **Over Sampling**
    - 광원 Projection frustum 영억 밖에 대해 depth map을 생성하면, [0, 1] 밖의 값을 가질 수 있다.
    - 기본 설정이면 **Over Sampling**이 발생하여 경계 밖 영역은 언제나 그림자가 발생한다.
    - 개선 방안: 텍스쳐 경계 설정
        - 텍스쳐 경계값을 0으로 설정하여 텍스쳐 경계에 그림자가 없도록 한다.
        - Far plane 너머는 무조건 0으로 설정한다.
1. **Pixelated Edges**
    - Depth map 해상도는 제한되어 있기 때문에 물체 경계에서 픽셀화되며 명확히 구분된 경계선이 생긴다.
    - 이는 실제 그림자가 물체 경계 영역에서 흐릿해지는 것과는 반대로 현실감을 낮춘다.
    - 개선 방안: **PCF(Percentage-Closer Filtering)**
        - 주변 텍셀의 값을 이용해서 3x3 영역의 평균을 사용하여 shadow factor를 계산한다.
        - shadow factor를 이용해서 컬러 값을 보정한다.

## Reference

- [udemy-opengl | directional shadow map](https://www.dropbox.com/scl/fi/qgbvv7s3880uvw994g8k0/slides_theory8_directional_shadow.pdf?rlkey=kpder9iufmeywah92a49mhf1n&dl=0)

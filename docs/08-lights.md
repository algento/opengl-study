# Lights

사실적인 그래픽 처리를 위해서 광원 효과를 잘 처리하는 것이 중요하다. 간단한 그래픽스 처리에서는 주로 `Phong Lighting  Model`을 사용한다.

## Phong Lighting

`Phong Lighting Model`은 다음의 3가지 모델을 조합으로 이뤄진다.

- `Ambient Lighting`:
  - 빛이 언제나 존재한다고 가정하는 가장 간단한 모델로, global illumination 효과를 보인다.
- `Diffuse Lighting`
  - 빛이 특정 광원에서 오는 것을 모델링한 것이다.
- `Specular Lighting`
  - 빛이 물체 표면에서 반사되어 관찰자의 눈으로 들어오는 효과를 모델링한 것이다.

![퐁 광원 모델](/docs/figs/phong-lighting.png)

빛이 태양과 같은 광원 (엄청나게 먼 곳에 있는 엄청나게 큰 광원)으로 부터 오는 빛을 모델링한 것이다.

### Ambient Lighting

- Ambinet Light은 가장 간단한 형태의 빛으로써, 다른 물체들에 의해 반사되어 일정 이상의 광량을 일관적으로 갖는 상황을 모델링한다.
  - e.g. 야외에서 햇빛을 손으로 가려서 생긴 그림자 영역이 완전하게 어둡지 않고 색이 보이는 것

#### Modeling of Ambient Lighting

- 쉐이더에서 현재의 컬러에 `ambientFactor`를 곱해서 모델링 한다.
- 결국 `ambientFactor`를 통해서 어둠의 정도를 조절하는 것이다.
  - `ambientFactor = 1`: 어두워 지는 것 없이 색이 그대로 빛난다.
  - `ambientFactor = 0`: 색이 하나도 나타나지 않고 빛이 없는 상태다.
- `fragColor = objectColor * (ambientFactor)`

### Diffuse Lighting

![디퓨즈 광원 모델](/docs/figs/diffuse_lighting.png)

- 광원과 물체의 표면과의 각도에 따라 밝기가 변하는 빛을 모델링한 것이다.
  - 광원과 물체의 표면이 수직이면 가장 빛나고 광원과 물체의 표면이 평행하면 해당 광원에 의한 빛 효과가 없다.
- 물체의 표면과 수직인 벡터를 surface normal 벡터라 한다.
![surface normal 벡터](/docs/figs/surface_normal_vector.png)
- OpenGL은 표면이 아니라 vertex롤 데이터를 관리하므로 여러 면이 맞닿아 있는 vertex는 여러 개의 normal 벡터를 가질 수 있다.
  - 위 그림의 좌측처럼 연속되지 않은 면을 가진 물체를 생각해보면, 한 꼭지점은 3개의 normal 벡터를 가질 수 있다.  

#### Phong Shading

![FLAT vs PHONG 쉐이딩](/docs/figs/flat_phong_shading.png)

- `Flat Shading`은 각 표면마다 하나의 normal 벡터를 갖도록 랜더링하는 것을 말한다. 이는 불연속적인 경계면이 있는 경우 적절하지만 연속된 표면을 모델링할 때는 부적절하다.
  - 또한 'Flat Shading`은 vertex가 여려 개의 normal을 가지고 잘 처리되어야 하므로 indexed draw와 잘 맞지 않는다.
- 이를 극복하기 위해서 `Phong Shading`을 사용한다.
  - `Phong Shading`은 하나의 vertex가 하나의 normal vector를 가지며, 그 값은 해당 vertex와 연관된 normal vector의 평균이다.
  - 랜더링할 때는 smooth effect를 표현하기 위해 이 평균값을 잘 보간해서 사용한다. 'Phong Shading`을 사용했을 때 나타나는 smoothness 효과는 이 보간 현상에 의해서 surface normal이 연속적으로 변하는 것처럼 처리함으로써 나타난다.

![노멀 벡터 변형](/docs/figs/normal_distortion_by_non-uniform_scaling.png)

- normal vector를 처리할 때 주의할 점은 non-uniform scale로 물체가 변환될 경우, 해당 변환을 normal vector에도 그대로 적용할 경우 실제로는 표면과 수직하지 않도록 변형된다는 점이다. (Wrongly skewing normals)
- Model 행렬에서 Rotation과 연관된 부분만 추출하여 normal 벡터를 변환해주면 된다.
  - $M = \begin{bmatrix} D_s R & D_s \vec{t} \\ 0 & 1\end{bmatrix} \rightarrow RD_s^{-1}$
  - $\vec{n}\cdot\vec{t} = \vec{n}^{\prime}\cdot\vec{l}^{\prime} = (G\vec{n})\cdot (M\vec{t}) = 0 \rightarrow G= (M^{-1})^T$
  - [참고 링크](http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/)

#### Modeling of Diffuse Lighting

- `diffuseFactor`  = $\vec{n} \cdot \vec{l}$
- $\vec{n}$: surface normal vector
- $\vec{l}$: vector from fragment to light source
- `diffuseFactor`가 음수이면 빛이 표면의 반대편에 있다는 것이므로 0으로 변경한다.
- `fragColor = objectColor * (ambientFactor + diffuseFactor)`

### Specular Lighting

![스페큘러 라이팅](/docs/figs/specular_lighting.png)

- `Specular Lighting`은 광원에서 방출된 빛이 물체의 표면에서 반사하여 관찰자의 눈에 밝게 빛나는 현상을 모델링한 것이다. 즉, 카메라의 움직임에 따라 빛 반사에 의해 빛나는 현상이 관측되는 것을 표현한 것이다.
- `Specular Lighting`을 계산하기 위해서는 다음의 정보가 필요하다.
  - Light vector
  - Normal vector
  - Reflection vector: 빛 반사 방향, built-in GLSL 함수 `reflect(incident, normal)`로 계산
  - Viewer vector: fragment 위치에서 카메라로의 방향 벡터
  - Shineness: 얼마나 빛이 많이 반사될 것인가에 대한 specular factor
    - Higher shineness: 작고 조밀한 반사
    - Lower shineness: 넓고 흐릿한 반사
- `specularFactor` = $(\vec{v}\cdot\vec{r})^{shineness}$
- `fragColor = objectColor * (ambientFactor + diffuseFactor + specularFactor)`

![물체의 shineness](/docs/figs/specular_lighting_shineness.png)

## Types of Lights

그래픽스에서 주로 다루는 빛의 종류는 다음과 같다.

- Directional Light: 태양과 같은 광원 (엄청나게 먼 곳에 있는 엄청나게 큰 광원)으로 부터 오는 빛을 모델링한 것
- Point Light: 특정 위치에서 사방으로 방출되는 형태의 빛을 모델링한 것
- Spot Light: 특정 위치에서 특정 범위로만 방출되는 형태의 빛을 모델링한 것

### Directional Light

태양과 같이 엄청나게 먼 곳에 있는 엄청나게 큰 광원으로 부터 방출되는 빛을 모델링한 것이기 때문에 월드 관점에서 빛이 특정 방향으로 부터 평행하게 온다. 따라서 이 타입의 빛을 모델링할 때는 빛이 오는 방향만 있으면 된다. (광원의 위치 불필요하다!)

![directional light](/docs/figs/directional_light.png)

#### Modeling of Directional Light

Directional light을 모델링하기 위해서 다음의 정보가 필요하다.

- Basic light information (color, ambientFactor, diffuseFactor, specularFactor)
- Direction
  - light vector가 월드 좌표계에서 주어지기 때문에 fragment에 따라 별도의 light vector를 계산할 필요가 없다.

### Point Light

Pont light은 점 형태의 광원으로 특정 위치에서 모든 방향으로 방출되는 형태의 빛을 말한다. 가장 대표적으로 백열 전구에서 방출되는 빛을 생각하면 이해하기 쉬울 것이다. 월드에서 그리고자 하는 객체에 해당 광원이 어떻게 영향을 끼치는지 계산하기 위해서 light vector를 fragment마다 계산해야 한다. light vector는 광원 위치에서 fragment 위치로 향하는 벡터이다.

![point light](/docs/figs/point-light.png)

#### Attenuation

현실적인 point light을 모델링하기 위해서는 빛의 attenuation을 고려해야 한다. 즉, 일반적으로 점 광원은 태양과 같은 타입의 빛과 달리 빛 에너지가 적기 때문에 거리에 따라 광량이 줄어들게 된다.백열 전구가 켜져 있는 영역에서 멀어지면 빛의 효과가 줄어드는 것을 상상해보면 이해하기 쉬울 것이다.
실제에서는 거리의 제곱에 반비례해서 광량이 줄어들기 때문에 다음과 같은 factor를 곱해서 attenuatoin을 모델링할 수 있다. 여기서 $d$는 광원과 fragment 사이의 거리이다. [(참고)](https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation)

$$f_{atten} = \frac{1}{(a\cdot d^2 + b\cdot d + c)} $$

### Spot Light

Spot light은 Point ligth과 유사하지만 특정 각도 영역에만 국한되어 방출되는 형태의 빛을 말한다. 가장 대표적으로 플래쉬를 상상하면 된다. 광원의 특성 자체는 point light과 동일하지만 일정 방향 (spotlight direction)과 cut-off angle을 가지고 특정 방향에 대해서 일정 각도 영역에서만 빛이 방출된다.
따라서 광원 위치와 fragment 사이의 벡터가 direction 벡터와 가지는 각도를 계산하고 그 각도가 cut-off angle 이내인지를 판단해야 한다.
![spot light](/docs/figs/spot-light.png)

`angleToFragment` = `lightVector` $\cdot$ `lightDirection`

- `lightVector`: 광원에서 fragment로 향하는 단위 벡터
- `lightDirection`: spot ligth이 지향하는 방향
- `angleToFragment`: `angle to fragment`의 cos 값으로 0에서 1사이의 값을 가진다. 해당 값이 크다는 것은 실제 각도는 작다는 의미이고 해당 값이 작다는 것은 각도는 크다는 의미가 된다. 따라서 이 값은 `cos(cutOffAngle)`과 비교해서 spot light 영역에 있는지 판단한다. (값이 더 크면 광원 방출 영역에 있는 것이다.)

#### Soft Edge

현실적인 spot light을 모델링하기 위해서는 cut-off angle의 edge 영역에서 빛이 흐려지는 효과를 내야 한다. 따라서 cut-off range에 있는 광량을 스케일링해줘야 한다. 하지만 단순한 스케일링은 효과를 보기 어렵다.
예를 들어 cut-off angle이 10도이면, `cos(cutOffAngle)`은 0.98이 되기 때문에 0.02의 range에서 soft edge 효과를 내기 어렵다. 따라서 다음과 같은 `spotLightFade` factor를 point light과 동일하게 만든 `spotLigthColor`에 곱해서 만든다.

`spotLightFade`= $1- \frac{1- \text{angleToFragment}}{1-\text{cutOffAngle}}$

### Area Light

Point ligth과 spot ligth이 점 형태의 광원에서 빛이 방출되는 것이라면, area light은 천정에 있는 LED 등처럼 일정 영역에서 빛을 방출하는 형태의 빛을 의미한다.

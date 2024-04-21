#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texcoord;
layout(location = 3) in vec3 a_normal; 

uniform mat4 u_view_projection_matrix;
uniform mat4 u_model_matrix;

out vec4 v_color;
out vec2 v_texture_coord;
// out vec3 v_world_position;
// out vec3 v_normal; //* vertex normals을 fragment shader로 넘겨줌.
flat out vec3 v_normal;//* flat shading을 하고 싶으면 이렇게 선언하면 됨. (interpolation을 하지 않음.)

void main() {
    v_texture_coord = a_texcoord;
    v_color = a_color;
    // v_world_position = (u_model_matrix * vec4(a_position, 1.0)).xyz;
    //* # Light 관련 문서 참고
    //* 1) 입력으로 받은 world 좌표계의 normal을 local 좌표계로 변환함. -> Normal = model * norm;
	//* 2) 단, 여기서 normal은 방향이므로 rotation만 반영하고 translation은 반영하면 안됨. 따라서 model matrix의 rotation 부분만을 이용하여 변환함. -> Normal = mat3(model) * norm;
	//* 3) 하지만 normal은 world에서 볼 때, scale invariant해야 함. scale 변화가 norm에 반영되므로 이를 상쇄해줘야 하기 때문에 아래와 같이 inverse를 transpose 해줘야 함. -> Normal = mat3(transpose(inverse(model))) * norm;
    v_normal = mat3(transpose(inverse(u_model_matrix))) * a_normal;
    gl_Position = u_view_projection_matrix * u_model_matrix * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;

in vec4 v_color;
in vec2 v_texture_coord;
// in vec3 v_normal;
flat in vec3 v_normal; //* flat shading을 하고 싶으면 이렇게 선언하면 됨. (interpolation을 하지 않음.)

struct Light {
    vec3 color;
    float ambient_intensity;
    vec3 direction;
    float diffuse_intensity;
};

uniform sampler2D u_texture;
uniform Light u_light;

vec4 CalculateLight(Light light) {
    vec4 ambient_color = vec4(light.color, 1.0F) * light.ambient_intensity;
    //* light direction과 normal을 정규화하여 내적을 구해서 diffuseFactor를 구한다.(0~1)
    float diffuse_factor = max(dot(normalize(v_normal), normalize(light.direction)), 0.0F);
    vec4 diffuse_color = vec4(light.color, 1.0F) * light.diffuse_intensity * diffuse_factor;
    
    return ambient_color + diffuse_color;
}

void main() {
    vec4 light_color = CalculateLight(u_light);
    o_color = texture(u_texture, v_texture_coord) * light_color ;
}
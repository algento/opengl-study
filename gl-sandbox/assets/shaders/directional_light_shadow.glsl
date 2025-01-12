//* 모든 것을 광원이 바라보는 방향에서 ortho로 projection하여 랜더링한다.
//* 어차피 shadow map 생성을 위한 depth map을 얻는 것이 주 목적이므로 color 필요없다.
#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
uniform mat4 u_model_matrix;
uniform mat4 u_directional_light_transform;

void main() {
    gl_Position = u_directional_light_transform * u_model_matrix * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

in vec4 v_color;
void main() {
}
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_view_projection_matrix;
// layout(std140) uniform Camera {
//     mat4 u_view_projection_matrix;
// };

uniform mat4 u_model_matrix;

out vec4 v_color;
void main() {
    gl_Position = u_view_projection_matrix * u_model_matrix * vec4(a_Position, 1.0);
    v_color = vec4(clamp(a_Position, 0.0f, 1.0f), 1.0f);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_color;
void main() {
    // color = vec4(1.0, 0.0, 0.0, 1.0);
    color = v_color;
}
#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_view_projection_matrix;
// layout(std140) uniform Camera {
//     mat4 u_view_projection_matrix;
// };

void main() {
    gl_Position = u_view_projection_matrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
    color = vec4(1.0, 0.0, 0.0, 1.0);
}
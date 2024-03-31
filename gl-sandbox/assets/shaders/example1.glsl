#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;

uniform mat4 u_view_projection_matrix;
uniform mat4 u_model_matrix;

out vec4 v_color;
out vec2 v_texture_coord;

void main() {
    gl_Position = u_view_projection_matrix * u_model_matrix * vec4(a_position, 1.0);
    v_texture_coord = a_texcoord;
    v_color = vec4(clamp(a_position, 0.0f, 1.0f), 1.0f);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;

in vec4 v_color;
in vec2 v_texture_coord;

uniform sampler2D u_texture;

void main() {
    o_color = texture(u_texture, v_texture_coord) * v_color;
    o_color.a  = 1.0f;
    // o_color = v_color;
}
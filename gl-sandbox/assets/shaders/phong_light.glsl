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
out vec3 v_frag_position;
out vec3 v_normal; //* vertex normals을 fragment shader로 넘겨줌.
// flat out vec3 v_normal;//* flat shading을 하고 싶으면 이렇게 선언하면 됨. (interpolation을 하지 않음.)

void main() {
    v_texture_coord = a_texcoord;
    v_color = a_color;
    // # Light 관련 문서 참고
    // 1. 입력으로 받은 world 좌표계의 normal을 local 좌표계로 변환함. -> Normal = model * norm;
	// 2. 단, 여기서 normal은 방향이므로 rotation만 반영하고 translation은 반영하면 안됨. 따라서 model matrix의 rotation 부분만을 이용하여 변환함. -> Normal = mat3(model) * norm;
	// 3. 하지만 normal은 world에서 볼 때, scale invariant해야 함. scale 변화가 norm에 반영되므로 이를 상쇄해줘야 하기 때문에 아래와 같이 inverse를 transpose 해줘야 함. -> Normal = mat3(transpose(inverse(model))) * norm;
    v_normal = mat3(transpose(inverse(u_model_matrix))) * a_normal;
    v_frag_position = (u_model_matrix * vec4(a_position, 1.0)).xyz;
    gl_Position = u_view_projection_matrix * u_model_matrix * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;

in vec4 v_color;
in vec2 v_texture_coord;
in vec3 v_normal;
// flat in vec3 v_normal; //* flat shading을 하고 싶으면 이렇게 선언하면 됨. (interpolation을 하지 않음.)
in vec3 v_frag_position;

struct DirectionalLight {
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
    vec3 direction;
};

// struct PointLight {
//     vec3 color;
//     float ambient_intensity;
//     float diffuse_intensity;
//     vec3 position;
//     vec3 coefficient;
// };

struct Material {
    float specular_intensity;
    float shineness;
};

uniform sampler2D u_texture;

uniform DirectionalLight u_directional_light;

uniform vec3 u_eye_position;

// const int MAX_POINT_LIGHTS = 3;
// uniform int u_num_point_lights;
// uniform PointLight u_point_light[MAX_POINT_LIGHTS];

uniform Material u_material;

vec4 CalculateLightFromDirection(vec3 color, float ambient_intensity, float diffuse_intensity, vec3 direction) {
    // ambient light
    vec4 ambient_color = vec4(color, 1.0F) * ambient_intensity;
    
    // diffuse light
    float diffuse_factor = max(dot(normalize(v_normal), normalize(direction)), 0.0F);
    vec4 diffuse_color = vec4(color, 1.0F) * diffuse_intensity * diffuse_factor;
    
    // specular light
    vec4 specular_color = vec4(0.0F, 0.0F, 0.0F, 0.0F);	
    if (diffuse_factor > 0.0F) { // 광원에서 물체이 빛이 들어올 때만 계산하는 것이다.
        // world 좌표계에서 현재 fragment의 위치에서 카메라의 위치로 향하는 방향 벡터를 계산한다.
        vec3 view_direction = normalize(u_eye_position - v_frag_position);
        // 광원에서 들어온 빛이 반사되어 나가는 방향을 계산한다.
        vec3 reflect_direction = reflect(direction, normalize(v_normal));
        float specular_factor = dot(view_direction, reflect_direction);
        
        if(specular_factor > 0.0F) {
            specular_factor = pow(specular_factor, u_material.shineness);
            specular_color = vec4(color * u_material.specular_intensity * specular_factor, 1.0F);
        }
    }
    
    return ambient_color + diffuse_color + specular_color;
}

vec4 CalculateDirectionalLight() {
    return CalculateLightFromDirection(u_directional_light.color, u_directional_light.ambient_intensity, u_directional_light.diffuse_intensity, u_directional_light.direction);
}

// vec4 CalculatePointLight() {
//     vec4 total_color = vec4(0.0F, 0.0F, 0.0F, 0.0F);
//     for (int i = 0; i < u_num_point_lights; i++) {
//         vec3 light_direction = v_frag_position - u_point_light[i].position;
//         float distance = length(light_direction);
//         light_direction = normalize(light_direction);
        
//         float attenuation = 1.0F / (u_point_light[i].coefficient.a * distance * distance + u_point_light[i].coefficient.b * distance + u_point_light[i].coefficient.c);
        
//         vec4 light_color = CalculateLightFromDirection(u_point_light[i].color, u_point_light[i].ambient_intensity, u_point_light[i].diffuse_intensity, light_direction);
//         total_color += (light_color / attenuation);
//     }
//     return totoal_color;
// }

void main() {
    vec4 directional_light_color = CalculateDirectionalLight();
    // vec4 point_light_color = CalculatePointLight();
    o_color = texture(u_texture, v_texture_coord) * (directional_light_color);
    // o_color = texture(u_texture, v_texture_coord) * (directional_light_color + point_light_color);
}
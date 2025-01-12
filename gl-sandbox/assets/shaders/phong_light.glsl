#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec3 a_normal; 

uniform mat4 u_view_projection_matrix;
uniform mat4 u_model_matrix;
uniform mat4 u_directional_light_transform;

out vec4 v_color;
out vec2 v_texture_coord;
out vec3 v_frag_position;
out vec3 v_normal; //* vertex normals을 fragment shader로 넘겨줌.
// flat out vec3 v_normal;//* flat shading을 하고 싶으면 이렇게 선언하면 됨. (interpolation을 하지 않음.)
out vec4 v_directional_light_position;

void main() {
    v_texture_coord = a_texcoord;
    v_color = a_color;
    // # Light 관련 문서 참고
    // 1. 입력으로 받은 world 좌표계의 normal을 local 좌표계로 변환함. -> Normal = model * norm;
    // 2. 단, 여기서 normal은 방향이므로 rotation만 반영하고 translation은 반영하면 안됨. 따라서 model matrix의 rotation 부분만을 이용하여 변환함. -> Normal = mat3(model) * norm;
    // 3. 하지만 normal은 world에서 볼 때, scale invariant해야 함. scale 변화가 norm에 반영되므로 이를 상쇄해줘야 하기 때문에 아래와 같이 inverse를 transpose 해줘야 함. -> Normal = mat3(transpose(inverse(model))) * norm;
    v_normal = mat3(transpose(inverse(u_model_matrix))) * a_normal;
    v_frag_position = (u_model_matrix * vec4(a_position, 1.0)).xyz;
    v_directional_light_position = u_directional_light_transform * u_model_matrix * vec4(a_position, 1.0);
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
in vec4 v_directional_light_position;

struct DirectionalLight {
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
    vec3 direction;
};

struct PointLight {
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
    vec3 position;
    vec3 attenuation_coefficient;
};

struct SpotLight {
    PointLight plight;
    vec3 direction;
    float cos_cutoff_angle;
};

struct Material {
    float specular_intensity;
    float shininess;
};

uniform sampler2D u_texture;
uniform sampler2D u_directional_shadow_map;
uniform DirectionalLight u_directional_light;

uniform vec3 u_eye_position;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

uniform int u_num_point_lights;
uniform PointLight u_point_light[MAX_POINT_LIGHTS];

uniform int u_num_spot_lights;
uniform SpotLight u_spot_light[MAX_SPOT_LIGHTS];

uniform Material u_material;

//* Shadow Factor를 계산하는 함수
float CalcDirectionalShadowFactor(DirectionalLight light) {
    //* NDC로 변환되므로 -1~+1사이로 변환된다.
    vec3 proj_coords = v_directional_light_position.xyz / v_directional_light_position.w;
    //* NDC를 0~1로 변환
    proj_coords = (proj_coords * 0.5) + 0.5;
    
    //* 텍스쳐를 통해 현재 위치의 깊이값을 가져온다.
    // float cloest = texture(u_directional_shadow_map, proj_coords.xy).r;
    //* 현재의 깊이 값을 가져온다.
    float current = proj_coords.z;
    //* 현재의 깊이값이 가장 가까운 깊이값보다 크다면 그것은 그림자가 있는 것이다.
    //* 하지만 이렇게 할 경우, far plane을 넘는 경우 벽처럼 그림자가 생기는 문제가 발생한다.
    // float shadow = current> cloest ? 1.0 : 0.0;
    // // float shadow = 1.0f;
    // if(proj_coords.z > 1.0) {
    //     shadow = 0.0;
    // }
    
    vec3 normal = normalize(v_normal);
    vec3 light_direction = normalize(light.direction);
    
    //* Bias를 사용하여 shadow acne가 생기는 문제를 해결한다.
    //* 카메라 방향에 따라 bias를 적용한다. 빛의 방향이 노멃방향과 유사할 때, 최소값이 적용되고 노멀방향과 수직일 때 최대값이 적용된다.
    float bias = max(0.05 * (1.0 - dot(normal, light_direction)), 0.0005);

    //* PCF를 사용하여 그림자를 부드럽게 한다. (한 개의 픽셀 값으로 그림자를 계산하는 것이 아니라 주변의 픽셀 값들을 사용하여 그림자를 계산한다.)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_directional_shadow_map, 0);  // texel 사이즈 계산
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_directional_shadow_map, proj_coords.xy + vec2(x,y) * texelSize).r;
            shadow += current - bias > pcfDepth ? 1.0 : 0.0;
            // float pcfDepth = texture(u_directional_shadow_map, proj_coords.xy).r;
            // shadow += current > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0; //* 9개의 픽셀 값들의 평균을 사용해서 그림자를 계산한다.
    
    if(proj_coords.z > 1.0)
    {
        shadow = 0.0;
    }
    
    return shadow;
}

vec4 CalculateLightWithDirection(vec3 color, float ambient_intensity, float diffuse_intensity, vec3 direction, float shadow_factor) {
    // ambient light
    vec4 ambient_color = vec4(color, 1.0F) * ambient_intensity;
    
    // diffuse light
    float diffuse_factor = max(dot(normalize(v_normal), normalize(direction)), 0.0F);
    vec4 diffuse_color = vec4(color * diffuse_intensity * diffuse_factor, 1.0F);
    
    // specular light
    vec4 specular_color = vec4(0.0F, 0.0F, 0.0F, 0.0F);	
    if (diffuse_factor > 0.0F) { // 광원에서 물체이 빛이 들어올 때만 계산하는 것이다.
        // world 좌표계에서 현재 fragment의 위치에서 카메라의 위치로 향하는 방향 벡터를 계산한다.
        vec3 view_direction = normalize(u_eye_position - v_frag_position);
        // 광원에서 들어온 빛이 반사되어 나가는 방향을 계산한다.
        vec3 reflect_direction = normalize(reflect(direction, normalize(v_normal)));
        float specular_factor = dot(view_direction, reflect_direction);
        
        if(specular_factor > 0.0F) {
            specular_factor = pow(specular_factor, u_material.shininess);
            specular_color = vec4(color * u_material.specular_intensity * specular_factor, 1.0F);
        }
    }
    
    return (ambient_color + (1.0 - shadow_factor) *(diffuse_color + specular_color));
}

vec4 CalculateDirectionalLight() {
    float shadow_factor = CalcDirectionalShadowFactor(u_directional_light);
    return CalculateLightWithDirection(u_directional_light.color, u_directional_light.ambient_intensity, u_directional_light.diffuse_intensity, u_directional_light.direction, shadow_factor);
}

vec4 CalculatePointLight(PointLight light) {
    vec3 vector_from_light_to_frag = v_frag_position - light.position;
    float distance = length(vector_from_light_to_frag);
    vec3 light_direction = normalize(vector_from_light_to_frag);
    
    float a = light.attenuation_coefficient.z; // quadratic
    float b = light.attenuation_coefficient.y; // linear
    float c = light.attenuation_coefficient.x; // constant
    float attenuation = 1.0F / (a * distance * distance + b * distance + c);
    
    vec4 light_color = CalculateLightWithDirection(light.color, light.ambient_intensity, light.diffuse_intensity, light_direction, 0.0);
    return light_color * attenuation;
}

vec4 CalculatePointLights() {
    vec4 total_color = vec4(0.0F, 0.0F, 0.0F, 0.0F);

    for (int i = 0; i < u_num_point_lights; i++) {
        total_color += CalculatePointLight(u_point_light[i]);
    }
    return total_color;
}

vec4 CalculateSpotLight(SpotLight light) {
    vec3 ray_direction = normalize(v_frag_position - light.plight.position);
    float cos_light_to_frag_angle = dot(ray_direction, light.direction);

    if (cos_light_to_frag_angle > light.cos_cutoff_angle) {
        //* light 원점에서 fragment까지의 벡터와 light의 방향 벡터 사이의 각도가 cutoff_value보다 작을 때!
        vec4 point_light_color = CalculatePointLight(light.plight);
        // return point_light_color * (1.0F - (1.0F - cos_light_to_frag_angle) / (1.0F - light.cos_cutoff_angle));
        return point_light_color * (1.0f - (1.0f - cos_light_to_frag_angle)*(1.0f/(1.0f - light.cos_cutoff_angle)));
    }
    else {
        return vec4(0.0F, 0.0F, 0.0F, 0.0F);
    }
}

vec4 CalculateSpotLights() {
    vec4 total_color = vec4(0.0F, 0.0F, 0.0F, 0.0F);
    for (int i = 0; i < u_num_spot_lights; i++) {
        total_color += CalculateSpotLight(u_spot_light[i]);
    }
    return total_color;
}

void main() {
    // vec4 total_color = vec4(0.0F, 0.0F, 0.0F, 0.0F);
    vec4 total_color = v_color;
    total_color += CalculateDirectionalLight();
    total_color += CalculatePointLights();
    total_color += CalculateSpotLights();
    
    o_color = texture(u_texture, v_texture_coord) * total_color;
}

// void main()
// {             
//     float shadow_factor = CalcDirectionalShadowFactor(u_directional_light);
//     o_color = vec4(vec3(shadow_factor), 1.0);
//     // float depth_value = texture(u_directional_shadow_map, v_texture_coord).r;
//     // o_color = vec4(vec3(depth_value), 1.0);
// } 
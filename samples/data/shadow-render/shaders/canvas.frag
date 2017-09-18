#version 330 core

uniform vec3 light_position;
uniform float light_radius;
uniform vec3 light_color;
uniform sampler2D diffuse_map;
uniform sampler2D normal_map;

in vec2 position;
in vec2 tex_coords;

out vec4 out_color;

void main() {
    vec3 normal = texture(normal_map, tex_coords.st).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 light_dir = normalize(light_position - vec3(position, 0.0));
    /*
    vec3 true_light_pos = light_position + light_radius * light_dir;
    vec3 true_light_dir = normalize(vec3(position, 0.0) - true_light_pos);
    */
    float intensity = dot(normal, light_dir);

    float distance = length(light_position - vec3(position, 0.0));

    vec3 dif_col = texture(diffuse_map, tex_coords.st).rgb;

    out_color = vec4(dif_col * light_color * intensity,
                     1.0);
}


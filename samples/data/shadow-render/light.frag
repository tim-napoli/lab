#version 330 core

in vec2 texel_position;

uniform vec3 light_position;
uniform vec3 light_color;

out vec4 out_color;

void main() {
    float dist = distance(vec3(texel_position, 0.0), light_position);
    float min_dist = light_position.z;
    float light_intensity = max(0.8, min_dist / dist);
    out_color = vec4(light_color * light_intensity, 1.0);
}


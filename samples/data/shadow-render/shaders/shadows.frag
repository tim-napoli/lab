#version 330 core

in vec2 tex_coords;
uniform sampler2D shadow_texture;
out vec4 out_color;

void main() {
    out_color = vec4(vec3(1.0) - texture(shadow_texture, tex_coords.st).rgb, 1.0);
}


#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D blur_1;
uniform sampler2D blur_2;
uniform sampler2D blur_3;

void main() {
    vec3 color_1 = texture(blur_1, tex_coords.st).rgb;
    vec3 color_2 = texture(blur_2, tex_coords.st).rgb;
    vec3 color_3 = texture(blur_3, tex_coords.st).rgb;
    vec3 color_mixed = (color_1 + color_2 + color_3) / 3.0;
    out_color = vec4(color_mixed, 1.0);
}


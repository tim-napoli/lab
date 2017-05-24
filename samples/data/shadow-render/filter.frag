#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D tex;

void main() {
    vec3 color = texture(tex, tex_coords.st).rgb;
    out_color = vec4(color, 1.0);
}


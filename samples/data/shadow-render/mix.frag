#version 330 core

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D tex;

void main() {
    out_color = vec4(texture(tex, tex_coords.st).rgb, 1.0);
}


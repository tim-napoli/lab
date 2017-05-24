#version 330 core

in vec2 tex_coords;
uniform sampler2D tex;
out vec4 out_color;

void main() {
    out_color = texture(tex, tex_coords.st);
}


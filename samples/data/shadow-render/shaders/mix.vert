#version 330 core

layout (location = 0) in vec2 pos_in;
layout (location = 1) in vec2 tex_in;

out vec2 tex_coords;

void main() {
    // In this shader, input must be in [-1.0, 1.0]
    gl_Position = vec4(pos_in, 0.0, 1.0);
    tex_coords = tex_in;
}


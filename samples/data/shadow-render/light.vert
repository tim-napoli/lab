#version 330 core

layout (location = 0) in vec2 pos_in;

uniform mat4 projection_matrix;

out vec2 texel_position;

void main() {
    gl_Position = projection_matrix * vec4(pos_in, 0.0, 1.0);
    texel_position = pos_in;
}


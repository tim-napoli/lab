#version 330 core

layout (location = 0) in vec2 pos_in;
layout (location = 1) in vec2 tex_coords_in;

uniform mat4 projection_matrix;
uniform mat4 transformation_matrix;

out vec2 tex_coords;

void main() {
    gl_Position = projection_matrix * transformation_matrix
                * vec4(pos_in, 0, 1.0);
    tex_coords = tex_coords_in;
}


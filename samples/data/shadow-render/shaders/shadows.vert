#version 330 core

layout (location = 0) in vec2 pos_in;
layout (location = 1) in vec2 tex_coords_in;

uniform mat4 projection_matrix;
uniform mat3 model_view_matrix;

out vec2 tex_coords;

void main() {
    vec3 position = model_view_matrix * vec3(pos_in, 1.0);
    gl_Position = projection_matrix * vec4(position, 1.0);
    tex_coords = tex_coords_in;
}


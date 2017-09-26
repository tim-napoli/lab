#version 330 core

layout (location = 0) in vec2 pos_in;
layout (location = 1) in vec2 tex_in;

uniform mat3 model_view_matrix;
uniform mat4 projection_matrix;

out vec2 position;
out vec2 tex_coords;

void main() {
    vec3 mv_pos = model_view_matrix * vec3(pos_in, 0.0);
    gl_Position = projection_matrix * vec4(mv_pos, 1.0);
    tex_coords = tex_in;
    position = pos_in;
}


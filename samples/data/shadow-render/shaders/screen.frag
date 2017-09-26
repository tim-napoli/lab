#version 330 core

in vec2 tex_coords;
out vec4 out_color;

uniform sampler2D canvas_texture;
uniform sampler2D shadows_texture;

void main() {
    vec3 canvas_col = texture(canvas_texture, tex_coords.st).rgb;
    vec3 shadows_col  = vec3(1.0) - texture(shadows_texture, tex_coords.st).rgb;
    out_color = vec4(canvas_col * shadows_col, 1.0);
}


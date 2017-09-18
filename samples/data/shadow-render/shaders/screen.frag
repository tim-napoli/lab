#version 330 core

in vec2 tex_coords;
out vec4 out_color;

uniform sampler2D canvas_tex;
uniform sampler2D shadows_tex;

void main() {
    vec3 canvas_col = texture(canvas_tex, tex_coords.st).rgb;
    vec3 shadows_col  = texture(shadows_tex, tex_coords.st).rgb;
    out_color = vec4(canvas_col * shadows_col, 1.0);
}


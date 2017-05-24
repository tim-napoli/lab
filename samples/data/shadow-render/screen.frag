#version 330 core

in vec2 tex_coords;
out vec4 out_color;

uniform sampler2D canvas_tex;
uniform sampler2D bloom_tex;

void main() {
    vec3 canvas_col = texture(canvas_tex, tex_coords.st).rgb;
    vec3 bloom_col  = texture(bloom_tex, tex_coords.st).rgb;
    out_color = vec4(4 * bloom_col + 0.5 * canvas_col, 1.0);
}


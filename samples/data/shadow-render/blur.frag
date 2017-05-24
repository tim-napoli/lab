#version 330 core

const float off = 1.0 / 500;

in vec2 tex_coords;
out vec4 out_color;
uniform sampler2D tex;

void main() {
    vec2 offsets[9] = vec2[](
        vec2(-off, off),
        vec2( 0.0, off),
        vec2( off, off),

        vec2(-off, 0),
        vec2( 0.0, 0),
        vec2( off, 0),

        vec2(-off, -off),
        vec2( 0.0, -off),
        vec2( off, -off)
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    vec3 col = vec3(0);
    for (int i = 0; i < 9; i++) {
        col += texture(tex, tex_coords.st + offsets[i]).rgb * kernel[i];
    }

    out_color = vec4(col, 1.0);
}


#version 330

// Input
in vec2 texcoord;

// Uniform
uniform sampler2D textures[10]; // MAX = 10
uniform int numTextures;
uniform float mix_factor;

// Output
layout(location = 0) out vec4 out_color;

void main() {
    vec4 color1 = texture(textures[0], texcoord);
    vec4 color2 = texture(textures[1], texcoord);

    vec4 mixedColor = mix(color1, color2, mix_factor);

    // alpha discard
    if (mixedColor.a < 0.5) {
        discard;
    }

    out_color = mixedColor;
}

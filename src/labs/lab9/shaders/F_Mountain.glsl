#version 330

// Input
in vec2 texCoords;

// Uniform
uniform sampler2D texture1;
uniform sampler2D texture2;

// Output
layout(location = 0) out vec4 out_color;

void main() {
    vec4 groundColor = texture(texture1, texCoords).rrrr;
    vec4 heightColor = texture(texture2, texCoords).rrrr;
    out_color = mix(groundColor, heightColor, 0.5);
}

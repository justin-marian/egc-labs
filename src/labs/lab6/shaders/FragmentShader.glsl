#version 330

// Input
//in vec3 fragment_postion;
in vec3 fragment_color;
in vec3 fragment_normal;
in vec2 fragment_texture_coord;

// Output
out vec4 out_color;

uniform float Time;

void main() {
    float red = 0.5 + 0.5 * sin(Time);
    float green = 0.5 + 0.5 * cos(Time);
    float blue = 0.5 + 0.5 * sin(Time*2) * cos(Time*2);
    out_color = vec4(red, green, blue, 1.0);
}

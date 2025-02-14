#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 2) in vec2 v_texture_coord;

// Uniform
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform sampler2D heightMap;

// Output
out vec2 texCoords;

void main() {
    texCoords = v_texture_coord;
    
    float height = texture(heightMap, texCoords).r;
    float incH = 5 * (height + 0.05);
    vec3 incHeight = v_position + vec3(0.0, incH, 0.0);

    gl_Position = Projection * View * Model * vec4(incHeight, 1.0);
}

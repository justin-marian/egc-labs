#version 330

// Input
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coord;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Time;

// Output
out vec3 fragment_position;
out vec3 fragment_color;
out vec3 fragment_normal;
out vec2 fragment_texture_coord;

void main()
{
    fragment_position = vertex_position;
    fragment_color = vertex_color;
    fragment_normal = vertex_normal;
    fragment_texture_coord = vertex_texture_coord;

    // TODO(student): Compute gl_Position
    // gl_Position =  Projection * View * Model * vec4(vertex_position, 1.0f);

    // OY MOVE RANGE OSCILATION TIME
    float angle = radians(45.0);
    mat4 rotationMatrix = mat4(
        cos(angle), -sin(angle), 0, 0,
        sin(angle),  cos(angle), 0, 0,
        0,           0,          1, 0,
        0,           0,          0, 1
    );
    vec4 posRot = rotationMatrix * vec4(vertex_position, 1.0f);
    vec3 pos = vec3(posRot.x, posRot.y + sin(Time), posRot.z);

    // Compute final position
    gl_Position = Projection * View * Model * vec4(pos, 1.0f);
}
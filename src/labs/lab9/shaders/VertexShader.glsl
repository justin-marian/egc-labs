#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
out vec2 texcoord;

uniform float time;

void main()
{
	texcoord = v_texture_coord;

	if (time >= 0)
	{
		texcoord.x += sin(time * 0.1) * 0.25;
	}

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}

#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;

// CT REFLEXION DIFUSSE
uniform float material_kd;
// CT REFLEXION SPECULAR
uniform float material_ks;
// CT REFLEXION SHINESS
uniform int material_shininess;
// CT COLOR OBJECT
uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	vec3 world_position = (Model * vec4(v_position, 1)).xyz;			// Position Vector
	vec3 P = world_position;											// P

	vec3 world_normal = normalize(mat3(Model) * normalize(v_normal));	// Normal Vector
	vec3 N = normalize(world_normal);									// N

	vec3 L = normalize(light_position - P);								// Vector DIR LIGHT		
	vec3 V = normalize(eye_position - P);								// Vector DIR EYE
	vec3 H = normalize(L + V);											// Vector MEDIAN
	vec3 R = reflect(-L, N);											// Vector DIR LIGHT REFLECTED

	//// ambientala = Ka * colorGlobalAmbiental;
	// define ambient light component
	float ambient_light = 0.25;

	//// difuza_lumina = Kd * culoare_lumina * max (dot(N,L), 0)
	// compute diffuse light component
	float diffuse_light = material_kd * max (dot(normalize(N), normalize(L)), 0);

	// compute specular light component
	float specular_light = 0;

	//// culoare_difuza = Ks * culoare_lumina * primeste_lumina * pow(max(dot(V, R), 0), n)
	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
	}

	float material_kc = 1;		// FACTOR ATTENUATION CONSTANT
	float material_kl = 0.14;	// FACTOR ATTENUATION LINEAR
	float material_kq = 0.07;	// FACTOR ATTENUATION SQUARED

	// Calculate the specular light with color
	vec3 color_light = vec3(0.0, 0.75, 0.75);
	vec3 specular_light_color = specular_light * color_light;

	//// factor_atenuare = 1/(kc + kl*d + Kq*d^2)
	// compute light
    float factor_aten = 1 / (
							material_kc +
							material_kl * distance(light_position, P) +
							material_kq * distance(light_position, P) * distance(light_position, P)
							);

	//// culoareObiect = ambientala + factorAtenuare * ( difuza + speculara ) (emisiva = 0)

    // Combine the color components and send color light output to fragment shader
    float light = ambient_light + factor_aten * (diffuse_light + specular_light);
    color = object_color * (light + specular_light_color);

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}

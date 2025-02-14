#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

uniform vec3 light_position[2];
uniform vec3 light_direction[2];
uniform vec3 light_color[2];

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

uniform int light_spot;
uniform float angle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // Define ambient, diffuse and specular light components
	vec3 V = normalize( eye_position - world_position );
    vec3 light[light_position.length];

    for (int i = 0; i < light_position.length; i++) {
        vec3 L = normalize( light_position[i] - world_position );
	    vec3 H = normalize( L + V );
	    vec3 R = reflect( -L, world_normal );

        float ambient_light = material_kd * 0.25;
        float diffuse_light = material_kd * max(dot(L, world_normal), 0);
        float specular_light = 0;

        // It's important to distinguish between "reflection model" and
        // "shading method". In this shader, we are experimenting with the Phong
        // (1975) and Blinn-Phong (1977) reflection models, and we are using the
        // Phong (1975) shading method. Don't mix them up!
        if (diffuse_light > 0)
        {
            specular_light = material_ks * pow(max(dot(R, V), 0), material_shininess);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        float cut_off = angle;
	    float spot_light = dot(-L, light_direction[i]);
	    float spot_light_limit = cos(cut_off); 

	    // Quadratic attenuation
	    float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
	    float light_att_factor = pow(linear_att, 2);
        /////////////////////////////////////////////////////////////////////////////////////////////

	    float aten_fac = 1 / pow(length(light_position[i] - world_position), 2);

        if(light_spot == 0) {
            light[i] += object_color * (ambient_light + aten_fac * (diffuse_light + specular_light));
        } else {
            if (spot_light > cos(cut_off))
		    {			
			    light[i] = object_color * (ambient_light + (light_att_factor + aten_fac) * (diffuse_light + specular_light));
		    } else {
			    light[i] = ambient_light * object_color;
		    }
        }
    }

    out_color = vec4(light[0] + light[1], 1);
}

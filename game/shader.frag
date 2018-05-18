#version 430

// Global variables for lighting calculations
layout(location = 1) uniform vec3 viewPos;
layout(location = 2) uniform sampler2D texShadow;
layout(location = 3) uniform float time;

layout(location = 4) uniform mat4 lightMVP;
layout(location = 5) uniform vec3 lightPos = vec3(3,3,3);

// Output for on-screen color
layout(location = 0) out vec4 outColor;

// Interpolated output data from vertex shader
in vec3 fragPos;    // World-space position
in vec3 fragNormal; // World-space normal

void main() {

	// Output the normal as color
	const vec3 lightDir = normalize(lightPos - fragPos);

	vec4 fragLightCoord = lightMVP * vec4(fragPos, 1.0);

	fragLightCoord.xyz /= fragLightCoord.w;

	// The resulting value is in NDC space (-1 to 1),
	// we transform them to texture space (0 to 1)
	fragLightCoord.xyz = fragLightCoord.xyz * 0.5 + 0.5;

	// Depth of the fragment with respect to the light
	float fragLightDepth = fragLightCoord.z;

	// Shadow map coordinate corresponding to this fragment
	vec2 shadowMapCoord = fragLightCoord.xy;

	// Shadow map value from the corresponding shadow map position
	float shadowMapDepth = texture(texShadow, shadowMapCoord).x;

	float visibility = 1.0;

	if ( shadowMapDepth + 0.00001 < fragLightDepth ) {
		visibility = 0;
	}

	outColor = visibility * vec4(vec3( max(dot(fragNormal, lightDir), 0.0)), 1.0);

}
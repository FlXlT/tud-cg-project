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

	float sum = 0.0;
	float iterations = 0.0;	
	float sampleArea = 0.01;
	float sampleSize = 0.002;
	float bias = 0.0000001;

	// Percentage Closer Filtering (PCF): averaging all neighbouring shadow test results.
	for (float y = -sampleArea; y <= sampleArea; y += sampleSize){
		for (float x = -sampleArea; x <= sampleArea; x += sampleSize){

			// Testing neighbour pixel
			const vec3 fragPosUnderTest = vec3(fragPos.x + x, fragPos.y + y, fragPos.z);

			vec4 fragLightCoord = lightMVP * vec4(fragPosUnderTest, 1.0);

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

			// The visibility factor, if it is 0 then a shadow was found
			float visibility = 1.0;

			// The shadow test, a small bias is added to avoid self-shadowing
			if ( shadowMapDepth + bias < fragLightDepth ) {
				visibility = 0.0;
			}
			sum += visibility;
			iterations += 1.0;
		}
	}
	
	const vec3 lightDir = normalize(lightPos - fragPos);

	outColor = (sum / iterations) * vec4(vec3( max(dot(fragNormal, lightDir), 0.0)), 1.0);
}
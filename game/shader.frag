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
in vec3 fragColor;
in vec2 texCoords;

void main() {

	// Shadow Variables
	bool inShadow = false;
	float sum = 0.0;
	float iterations = 0.0;	
	float sampleArea = 0.001;
	float sampleSize = 0.0002;
	float bias = 0.001;
	float visibility_factor = 0.2;

	// Shading Variables
	float shininess = 64;
	
	vec4 fragLightCoord = lightMVP * vec4(fragPos, 1.0);

	fragLightCoord.xyz /= fragLightCoord.w;

	// The resulting value is in NDC space (-1 to 1),
	// we transform them to texture space (0 to 1)
	fragLightCoord.xyz = fragLightCoord.xyz * 0.5 + 0.5;

	// Depth of the fragment with respect to the light
	float fragLightDepth = fragLightCoord.z;

	// Shadow map coordinate corresponding to this fragment
	vec2 shadowMapCoord = fragLightCoord.xy;

	// Percentage Closer Filtering (PCF): averaging all neighbouring shadow test results.
	for (float y = -sampleArea; y <= sampleArea; y += sampleSize){
		for (float x = -sampleArea; x <= sampleArea; x += sampleSize){

			vec2 shadowMapCoordUnderTest = vec2(shadowMapCoord.x + x, shadowMapCoord.y + y);

			// Shadow map value from the corresponding shadow map position
			float shadowMapDepth = texture(texShadow, shadowMapCoordUnderTest).x;

			// The visibility factor, if it is 0 then a shadow was found
			float visibility = 1.0;

			// The shadow test, a small bias is added to avoid self-shadowing
			if ( shadowMapDepth + bias < fragLightDepth ) {
				visibility = visibility_factor;
				inShadow = true;
			}
			sum += visibility;
			iterations += 1.0;
		}
	}
	
	//vec3 lightPosVariance = vec3(sin(time)*3, 3.0, cos(time)*3);
	vec3 lightDir = normalize(lightPos - fragPos);

	// Diffuse shading
	vec3 Kd = fragColor;
	const float diffuseFactor = dot(lightDir, fragNormal);
	vec3 diffuse = Kd * diffuseFactor;
	diffuse = clamp(diffuse, vec3(0, 0, 0), vec3(1.0, 1.0, 1.0));

	// Specular shading
	// Compute specular color component
	vec3 specular;
	
	if(!inShadow){
		vec3 L = lightDir - fragPos;
		vec3 V = viewPos - fragPos;
		vec3 H = (L + V) / length(L + V);
		vec3 Ks = vec3(1.0, 1.0, 1.0);
		float specularFactor = pow(dot(fragNormal, H), shininess);
		specular = Ks * specularFactor;
	} else {
		specular = vec3(0,0,0);
	}
	
	outColor =  (sum / iterations) * vec4(diffuse + specular, 1.0);
}
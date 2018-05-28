#version 430

// Model/view/projection matrix
layout(location = 0) uniform mat4 mvp;
layout(location = 1) uniform mat4 model;

// Per-vertex attributes
layout(location = 0) in vec3 pos; // World-space position
layout(location = 1) in vec3 normal; // World-space normal
layout(location = 2) in vec3 color; // Color
layout(location = 3) in vec2 texCoords; // Texture coordinates

// Data to pass to fragment shader
out vec3 fragPos;
out vec3 fragNormal;
out vec3 fragColor;
out vec2 fragTexCoords;

void main() {
	// Transform 3D position into on-screen position
    gl_Position = mvp * vec4(pos, 1.0);


    // Transform vertex position using model matrix and pass to fragment shader
    fragPos = vec3(model * vec4(pos.xyz, 1));


	// Transform vertex normal using model matrix and pass to fragment shader
	// I thought we did not need transpose-inverse in case we want to support non-uniform-scaling:
	// https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
	// But could not get it to work otherwise, so applied it anyway, also inspired by:
	// https://gamedev.stackexchange.com/questions/112494/transforming-normals-along-with-vertex-glsl
    
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	fragNormal = normalize(normalMatrix * normal);

	// Alternative that did not yield the proper result
	// (Interesting effect though, uncomment for some fun)
	//fragNormal = normalize(vec3(model * vec4(normal.xyz, 1)));

	fragColor = color;
	fragTexCoords = texCoords;
}
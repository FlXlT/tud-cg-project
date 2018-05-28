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

    // Pass position and normal through to fragment shader
    fragPos = vec3(model * vec4(pos.xyz, 1));
	//fragPos = pos;

	// In case we want to support non-uniform-scaling:
	// https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
	// Otherwise, directly applying model matrix and normalizing is enough:
	// https://gamedev.stackexchange.com/questions/112494/transforming-normals-along-with-vertex-glsl
    //fragNormal = normalize(vec3(model * vec4(normal.xyz, 1)));
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	fragNormal = normalize(normalMatrix * normal);
	//fragNormal = normal;
}
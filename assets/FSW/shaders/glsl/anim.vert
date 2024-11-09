#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNorm;
layout (location = 2) in vec2 vertUv;

//layout (location = 3) in vec4 vertColor;

layout (location = 3) in vec4 weights;
layout (location = 4) in ivec4 boneIds;

layout (std140) uniform stdVert {
	mat4 model;
	mat4 view;
	mat4 proj;
	mat4 normMat;
};

#define MAX_BONE 32
layout (std140) uniform animData {
	mat4 finalBoneMatrices[MAX_BONE];
};

out vec3 fragPos;
out vec3 normal;
out vec2 uv;
out vec4 color;


void doAnimBone(inout vec4 totalPosition, inout vec3 totalNormal, vec3 pos, vec3 norm, int boneId, float weight) {
	if (boneId >= 0 && boneId < MAX_BONE) {
		totalPosition += (finalBoneMatrices[boneId] * vec4(pos, 1)) * weight;
		totalNormal += mat3(finalBoneMatrices[boneId]) * norm * weight;
	}
}

void main() {
	vec4 totalPosition = vec4(0.0f);
	vec3 totalNormal = vec3(0.0f);
	doAnimBone(totalPosition, totalNormal, vertPos, vertNorm, boneIds.x, weights.x);
	doAnimBone(totalPosition, totalNormal, vertPos, vertNorm, boneIds.y, weights.y);
	doAnimBone(totalPosition, totalNormal, vertPos, vertNorm, boneIds.z, weights.z);
	doAnimBone(totalPosition, totalNormal, vertPos, vertNorm, boneIds.w, weights.w);

	vec4 p = view * (model * vec4(totalPosition.xyz, 1));

	fragPos = p.xyz;
	gl_Position = proj * p;
	normal = (normMat * vec4(totalNormal, 1)).xyz;
	uv = vertUv;
	color = vec4(0.5, 0.5, 0.5, 1);
}
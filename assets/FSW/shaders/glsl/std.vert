#version 330 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNorm;
layout (location = 2) in vec2 vertUv;
layout (location = 3) in vec4 vertColor;

layout (std140) uniform stdVert {
	mat4 model;
	mat4 view;
	mat4 proj;
	mat4 normMat;
};

out vec3 fragPos;
out vec3 normal;
out vec2 uv;
out vec4 color;

void main() {
	vec4 p = view * (model * vec4(vertPos, 1));
	fragPos = p.xyz;
	gl_Position = proj * p;
	normal = (normMat * vec4(vertNorm, 1)).xyz;
	uv = vertUv;
	color = vertColor;
}
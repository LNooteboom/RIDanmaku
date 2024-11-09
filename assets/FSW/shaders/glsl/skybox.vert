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

out vec3 uvw;

void main() {
	gl_Position = proj * vec4(mat3(view) * vertPos, 1);
	uvw = vec3(vertPos.x, -vertPos.z, vertPos.y);
}

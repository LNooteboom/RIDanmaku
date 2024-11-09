#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 uv;
in vec4 color;

#define N_POINT	8
#define N_TEX 8
struct Texture {
	vec2 off;
	vec2 scale;
};
struct Light {
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	int u1;
};
layout (std140) uniform stdFrag {
	Light dirLight;
	Light pointLights[N_POINT];
	Texture tex[N_TEX];
	float glossiness;
	float fogMin;
	float fogMax;
	vec3 fogColor;
};
uniform sampler2D texBase;

void main() {
	vec4 base = texture(texBase, uv * tex[0].scale + tex[0].off) * color;
	float gamma = 2.2f;
	vec3 col = pow(base.xyz, vec3(1.0f/gamma));
	FragColor = vec4(col, base.w);
}
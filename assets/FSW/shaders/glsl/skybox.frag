#version 330 core
out vec4 FragColor;

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

uniform samplerCube skybox;

in vec3 uvw;

void main()
{
	FragColor = texture(skybox, uvw);
	//FragColor = vec4(uvw, 1);
}

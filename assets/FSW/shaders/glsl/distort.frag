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
uniform sampler2D tex1;

void main() {
	vec4 vcol = clamp(color, 0, 1);
	float distort = (texture(tex1, uv * tex[1].scale + tex[1].off).x - 0.5f) * 0.1 * vcol.w;
	vec2 max = vec2(384, 448) / textureSize(texBase, 0);
	vec2 distUv = clamp(uv * tex[0].scale + tex[0].off + distort, vec2(0), max);
	vec3 base = texture(texBase, distUv).xyz * vcol.xyz;
	float gamma = 2.2f;
	vec3 col = pow(base, vec3(1.0f/gamma));
	FragColor = vec4(col, 1);
}
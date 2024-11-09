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


float calcDiffuse(vec3 normal, vec3 lightDir) {
	return dot(normal, lightDir);
}

float calcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir) {
	vec3 reflectDir = reflect(-lightDir, normal);
	return pow(max(dot(viewDir, reflectDir), 0.0), glossiness);
}

vec3 calcDirLight(Light light, vec3 normal, vec3 base, vec3 viewDir) {
	vec3 lightDir = normalize(light.pos);
	float diff = calcDiffuse(normal, lightDir);
	float spec = calcSpecular(normal, lightDir, viewDir);

	vec3 ambient = light.ambient * base;
	vec3 diffuse = light.diffuse * diff * base;
	vec3 specular = light.specular * spec * base;

	return ambient + diffuse + specular;
	//return ambient + diffuse;
}
vec3 calcPointLight(Light light, vec3 fragPos, vec3 normal, vec3 base, vec3 viewDir) {
	if (light.constant < 0.9)
		return vec3(0);
	vec3 lightDir = normalize(light.pos - fragPos);
	float diff = calcDiffuse(normal, lightDir);
	float spec = calcSpecular(normal, lightDir, viewDir);

	float distance = length(light.pos - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * base;
	vec3 diffuse = light.diffuse * diff * base;
	vec3 specular = light.specular * spec;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 doFog(vec3 color) {
	float f = clamp((fogMax + fragPos.z) / (fogMax - fogMin), 0, 1);
	return mix(fogColor, color, f);
}

void main() {
	vec4 base = texture(texBase, uv * tex[0].scale + tex[0].off);
	base.xyz *= 2 * color.xyz;
	base.w = base.w * color.w;

	vec3 viewDir = normalize(-fragPos);
	vec3 result = calcDirLight(dirLight, normal, base.xyz, viewDir);
	for (int i = 0; i < N_POINT; i++) {
		//result += calcPointLight(pointLights[i], fragPos, normal, base.xyz, viewDir);
	}

	FragColor = vec4(doFog(result), base.w);
}

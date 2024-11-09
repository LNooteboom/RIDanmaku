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
	vec3 specular = light.specular * spec;

	return ambient + diffuse + specular;
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
	if (fragPos.z > -4) {
		float z = fragPos.z / -4;
		vec2 fp = gl_FragCoord.xy / 2;
		vec2 d = (fp - floor(fp));
		if (z > 0.5) {
			if (d.x >= 0.5 && d.y >= 0.5)
				discard;
		} else if (z > 0.25) {
			if (d.x != d.y)
				discard;
		} else if (z > 0.125) {
			if (d.x >= 0.5 || d.y >= 0.5)
				discard;
		} else {
			discard;
		}
	}

	vec4 base = texture(texBase, uv * tex[0].scale + tex[0].off);
	base.w = base.w * color.w;
	if (base.w < 0.5) {
		discard;
	}

	base.xyz *= 2 * color.xyz;
	
	//vec3 viewDir = normalize(-fragPos);
	//vec3 result = calcDirLight(dirLight, normal, base.xyz, viewDir);
	//for (int i = 0; i < N_POINT; i++) {
	//	result += calcPointLight(pointLights[i], fragPos, normal, base.xyz, viewDir);
	//}
	vec3 result = (dirLight.ambient * base.xyz) + (0.5 * dirLight.diffuse * base.xyz);

	FragColor = vec4(doFog(result), 1);
}
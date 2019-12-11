#version 330 core
in vec3 FragPos;
in vec3 Normal;

uniform vec3 color;
uniform vec3 eye;

out vec4 fragColor;

void main()
{
    vec3 diffuse = vec3(0.5, 0.5, 0.5);
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    vec3 lightVec = normalize(eye - FragPos);
	vec3 eyeVec = normalize(FragPos - eye);
    vec3 phong =  diffuse * (dot(lightVec, Normal)) + ambient;

    float attenuation = 1.0 / (1.2 * length(eye - FragPos));
    phong *= attenuation;
    fragColor = vec4(phong, 1);
}
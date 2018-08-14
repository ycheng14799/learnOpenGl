#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light { 
	vec3 position;

	vec3 ambient; 
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

uniform Material material; 
uniform Light light; 
uniform vec3 viewPos; 

in vec3 Normal; 
in vec3 FragPos; 
in vec2 TexCoords; 

out vec4 FragColor;

void main() {

	// Determine ambient light 
	// Set equal to diffuse material's color 
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb; 
	
	// Determine diffuse lighting 
	// Determine norm and lighting direction
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	// Calculate actual diffuse impact 
	float diff = max(dot(norm, lightDir), 0.0);
	// Retrieve fragment diffuse colors by multiplying with fragment's diffuse color 
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 
	
	// Determine specular lighting 
	vec3 viewDir = normalize(viewPos - FragPos);
 	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
	
	// Factor in attenuation 
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	
	// Calculate result
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}

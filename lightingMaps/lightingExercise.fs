#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission; 
	float shininess;
};

struct Light { 
	vec3 position;
	
	vec3 ambient; 
	vec3 diffuse;
	vec3 specular;
};

uniform Material material; 
uniform Light light; 
uniform vec3 viewPos; 
uniform float emissionBrightness; 

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
	
	// Determine emission
	vec3 emission = emissionBrightness * texture(material.emission, TexCoords).rgb;

	// Calculate result
	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
}

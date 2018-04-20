#version 330 core

struct Light {
    vec3 pos_dir;

	int type;
	bool on;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



in vec3 FragPos;
in vec3 Normal;


out vec4 color;

#define NR_LIGHTS 10

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Light lights[NR_LIGHTS];
uniform float objectShininess;


// Function prototypes
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result;

	for(int i = 0; i < NR_LIGHTS; i++){
  	if(lights[i].on == true){
	   if(lights[i].type == 1)
           result = CalcDirLight(lights[i], norm, viewDir);
	   if(lights[i].type == 0)
	       result += CalcPointLight(lights[i], norm, FragPos, viewDir);    
	   }
	}   
    color = vec4(result, 1.0);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.pos_dir);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), objectShininess);
    // Combine results
    vec3 ambient = light.ambient * objectColor * 0.25;
    vec3 diffuse = light.diffuse * diff * objectColor * 0.25;
    vec3 specular = light.specular * spec * objectColor * 0.25;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pos_dir - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), objectShininess);
    // Attenuation
    float distance = length(light.pos_dir - fragPos);
    // Combine results
    vec3 ambient = light.ambient * objectColor * 0.25;
    vec3 diffuse = light.diffuse * diff * objectColor * 0.25;
    vec3 specular = light.specular * spec * objectColor * 0.25; 
    return (ambient + diffuse + specular);
}
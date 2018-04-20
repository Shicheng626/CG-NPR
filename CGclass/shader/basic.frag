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
in vec2 TexCoords;
in mat3 TBN;

out vec4 color;

#define NR_LIGHTS 10

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Light lights[NR_LIGHTS];
uniform float objectShininess;

uniform bool texture_on;
uniform bool normal_on;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;


vec3 pure_color;
// Function prototypes
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // Properties

	vec3  norm = normalize(Normal);

 	if(normal_on){
	    norm = texture(normalTexture, TexCoords).rgb;
		norm = normalize(norm * 2.0 - 1.0); 
	}

    vec3 TViewPos = TBN * viewPos;
    vec3 TFragPos = TBN * FragPos;



    vec3 viewDir = normalize(TViewPos - TFragPos);

	if(texture_on){
	pure_color =vec3(texture(diffuseTexture, TexCoords));
	}else{
	pure_color = objectColor;
	}

    vec3 result;

	for(int i = 0; i < NR_LIGHTS; i++){
  	if(lights[i].on == true){
	   if(lights[i].type == 1)
           result = CalcDirLight(lights[i], norm, viewDir);
	   if(lights[i].type == 0)
	       result += CalcPointLight(lights[i], norm, TFragPos, viewDir);    
	   }
	}  
    color = vec4(result, 1.0);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-TBN*light.pos_dir);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), objectShininess);
    // Combine results
    vec3 ambient = light.ambient * pure_color * 0.1;
    vec3 diffuse = light.diffuse * diff * pure_color * 0.5;
    vec3 specular = light.specular * spec * pure_color * 0.2;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(TBN*light.pos_dir - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), objectShininess);
    // Combine results
    vec3 ambient = light.ambient * pure_color * 0.1;
    vec3 diffuse = light.diffuse * diff * pure_color * 0.5;
    vec3 specular = light.specular * spec * pure_color * 0.2;
    return (ambient + diffuse + specular);
}
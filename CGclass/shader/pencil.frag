#version 330 core

struct Light {
    vec3 pos_dir;

	int type;
	bool on;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
/*

*/
//in VS_OUT {
//    vec3 Normal;
 //   vec3 FragPos;
 //   vec4 ScrPos;
 //   vec2 TexCoords;
 //   mat3 TBN;
//} fs_in;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 ScrPos;
in mat3 TBN;
in vec2 PencilTexCoords;


out vec4 color;


#define NR_LIGHTS 10

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Light lights[NR_LIGHTS];
uniform float objectShininess;

uniform bool texture_on;
uniform bool normal_on;

uniform sampler2D normalTexture;

uniform sampler2D level0;
uniform sampler2D level1;
uniform sampler2D level2;
uniform sampler2D level3;
uniform sampler2D level4;
uniform sampler2D level5;

uniform float tile_factor;
uniform bool inface_blending;

vec3 pure_color;

// Function prototypes
float CalcDirLight(Light light, vec3 normal, vec3 viewDir);
float CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 getPencilLevel(float diff,vec2 scrPos,bool blending){
if(blending){
    diff = diff*7;
	if(diff>6){
	return vec3(1,1,1);
	}else if(diff>5){
	return mix(texture(level0, scrPos).rgb,vec3(1,1,1),diff-5);
	}
	else if(diff>4){
	return mix(texture(level1, scrPos).rgb,texture(level0, scrPos).rgb,diff-4);
	}
	else if(diff>3){
	return mix(texture(level2, scrPos).rgb,texture(level1, scrPos).rgb,diff-3);
	}
	else if(diff>2){
	return mix(texture(level3, scrPos).rgb,texture(level2, scrPos).rgb,diff-2);
	}
	else if(diff>1){
	return mix(texture(level4, scrPos).rgb,texture(level3, scrPos).rgb,diff-1);
	}else{
	return mix(texture(level5, scrPos).rgb,texture(level4, scrPos).rgb,diff);
	}
	}
	else{
	 diff = diff*7;
	if(diff>6){
	return vec3(1,1,1);
	}else if(diff>5){
	texture(level0, scrPos).rgb;
	}
	else if(diff>4){
	return texture(level1, scrPos).rgb;
	}
	else if(diff>3){
	return texture(level2, scrPos).rgb;
	}
	else if(diff>2){
	return texture(level3, scrPos).rgb;
	}
	else if(diff>1){
	return texture(level4, scrPos).rgb;
	
	}else{
	return texture(level5, scrPos).rgb;
	
	}
	}
}

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

    float result;

	for(int i = 0; i < NR_LIGHTS; i++){
  	if(lights[i].on == true){
	   if(lights[i].type == 1)
           result = CalcDirLight(lights[i], norm, viewDir);
	   if(lights[i].type == 0)
	       result += CalcPointLight(lights[i], norm, TFragPos, viewDir);    
	   }
	}  

	if(texture_on){
	
	  // vec2 new_scr_pos = (vec2(ScrPos.x,ScrPos.y))/ScrPos.z * 2;
	  vec2 new_scr_pos = (vec2(PencilTexCoords.x,PencilTexCoords.y))/ScrPos.z * tile_factor;
	   pure_color = getPencilLevel(result,new_scr_pos,inface_blending);
    	
	}else{
	   pure_color = objectColor;
	}

    color = vec4(pure_color, 1.0);
}


// Calculates the color when using a directional light.
float CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-TBN*light.pos_dir);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), objectShininess);

    return (0.1 + diff  * 0.5 + spec  * 0.2);
}

// Calculates the color when using a point light.
float CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(TBN*light.pos_dir - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, normal), 0.0), objectShininess);
    // Combine results
      return (0.1 + diff  * 0.5 + spec  * 0.2);
}
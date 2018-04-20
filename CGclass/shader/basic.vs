#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool normal_on;

void main()
{
  
	if(normal_on){
	mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);  

	TBN = transpose(mat3(T, B, N));  
	}else{
	
	TBN = mat3(1.0f);
	}
    gl_Position = projection * view *  model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;  
	TexCoords = vec2(texCoords.x,1-texCoords.y);

} 
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;
out vec4 ScrPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position =  view *  model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;  
    Normal = normalize(Normal);
	Normal.z = 0;
	gl_Position = gl_Position + vec4(Normal, 0)* 0.02;
	gl_Position = projection * gl_Position;
	ScrPos = gl_Position;
} 
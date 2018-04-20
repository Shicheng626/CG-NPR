#version 330 core

in vec4 ScrPos;

out vec4 color;

uniform sampler2D level5;

void main()
{    
	vec2 new_scr_pos = (vec2(ScrPos.x,ScrPos.y))/ScrPos.z * 3;
	color = texture(level5, new_scr_pos);
    //color = vec4(0.15,0.15,0.15, 1.0);
}

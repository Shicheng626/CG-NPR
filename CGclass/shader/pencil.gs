#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec4 ScrPos;
    vec2 TexCoords;
    mat3 TBN;
} gs_in[];

 out vec3 Normal;
 out vec3 FragPos;
 out vec4 ScrPos;
 out vec2 TexCoords;
 out mat3 TBN;
 out vec2 PencilTexCoords;

 uniform bool align2normal;
 uniform float max;
 uniform float k;

 void set_gs_output(int i){
       Normal = gs_in[i].Normal;
	  FragPos = gs_in[i].FragPos;
	  ScrPos = gs_in[i].ScrPos;
	 TexCoords = gs_in[i].TexCoords;
	 TBN = gs_in[i].TBN;
 }

 vec3 GetNormal()
{
   vec3 a = vec3(gs_in[0].ScrPos) - vec3(gs_in[1].ScrPos);
   vec3 b = vec3(gs_in[2].ScrPos) - vec3(gs_in[1].ScrPos);
   return normalize(cross(a, b));
}

void main()
{


vec3 scrNormal = GetNormal();
vec2 Normal2d = normalize(vec2(scrNormal.x,scrNormal.y));
float angle =acos(dot(Normal2d,vec2(0,1)));
angle = max/(1+exp(-k*angle))-max/2;



for(int i = 0 ;i<3;i++){
if(align2normal){
      PencilTexCoords.x = gs_in[i].ScrPos.x * cos(angle) - gs_in[i].ScrPos.y * sin(angle);
	  PencilTexCoords.y = gs_in[i].ScrPos.y * cos(angle) + gs_in[i].ScrPos.x * sin(angle);
}else{

 PencilTexCoords.x = gs_in[i].ScrPos.x;
PencilTexCoords.y = gs_in[i].ScrPos.y;
}
      set_gs_output(i);
      gl_Position = gl_in[i].gl_Position;
      EmitVertex();
	 
	  }
	   EndPrimitive(); 
}
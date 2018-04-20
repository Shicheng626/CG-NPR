#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec4 ScrPos;
} gs_in[];

const float MAGNITUDE = 0.2f;

 vec3 GetNormal()
{
   vec3 a = vec3(gs_in[0].ScrPos) - vec3(gs_in[1].ScrPos);
   vec3 b = vec3(gs_in[2].ScrPos) - vec3(gs_in[1].ScrPos);
   return normalize(cross(a, b));
}

void GenerateLine()
{
    vec4 center = (gs_in[0].ScrPos + gs_in[1].ScrPos + gs_in[2].ScrPos)/3.0f;
    gl_Position = center ;
    EmitVertex();
    gl_Position = center + vec4(GetNormal(), 0.0f) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(); 

}
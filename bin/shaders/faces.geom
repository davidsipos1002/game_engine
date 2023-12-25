#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 gPosition[];
in vec3 gNormal[];

out vec3 fPosition;
out vec3 fNormal;

void main()
{
    fNormal = gNormal[0];
    for (int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        fPosition = gPosition[i];
        EmitVertex();
    }
    EndPrimitive();
}
#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 gPosEye[];
in vec3 gNormalEye[];
in vec3 gLightDirection[];

out vec3 fPosEye;
out vec3 fNormalEye;
flat out vec3 fLightDirection;

void main()
{
    fLightDirection = gLightDirection[0];
    fNormalEye = gNormalEye[0];
    for (int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        fPosEye = gPosEye[i];
        EmitVertex();
    }
    EndPrimitive();
}
// type = vertex, name = dlv

#version 450 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normMat;

out vec3 Normal;
out vec3 FragPos;
void main()
{
    gl_Position = proj*view*model*vec4(pos, 1);
    /* Normal = normMat * norm; */
    Normal = mat3(transpose(inverse(model))) * norm;
    FragPos = pos;
}

// type = fragment, name = dlf

#version 450 core

#pragma pxe_include_shader "lighting/ambient.glsl"

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
uniform vec3 dirsource;
uniform vec4 color;
uniform AmbientLight ac;
void main()
{
    float df = max(dot(-Normal, dirsource - FragPos), 1);
    vec4 dc = vec4(1, 1, 0.956862745, 1) * df * 2;
    FragColor = color*(getAmbient(ac) + dc);
}

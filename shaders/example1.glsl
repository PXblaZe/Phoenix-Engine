// type = vertex, name = v

#version 330 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec4 col;
layout(location=2) in vec3 norm;
layout(location=3) in vec2 tex;

out vec3 FragPos;
out vec4 color;
out vec3 Normal;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(vertex, 1);
    color = col;
    Normal = norm;
    texCoord = tex;
    FragPos = vertex;
}


// type = fragment, name = f

#version 330 core

#pragma pxe_include_shader "lighting/ambient.glsl"

in vec3 FragPos;
in vec4 color;
in vec3 Normal;
in vec2 texCoord;
out vec4 Color;

uniform vec3 dirsource;
uniform bool useMix;
uniform sampler2D theTex;
uniform AmbientLight ambient;

void main() 
{
    Color = texture(theTex, texCoord);
    if (useMix) Color = mix(Color, color, .5);
    float diffuseFactor = max(dot(Normal, normalize(dirsource - FragPos)), 0);
    vec4 diffuse = vec4(vec3(1, 1, 1)*diffuseFactor*2, 1);
    Color *= (getAmbient(ambient) + diffuse);
}

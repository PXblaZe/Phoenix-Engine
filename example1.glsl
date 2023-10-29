// type = vertex, name = v

#version 450 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec4 col;

out vec4 color;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(vertex, 1);
    color = bool(col.a) ? col : vec4(col.rgb, 1);
}


// type = fragment, name = f

#version 450 core

in vec4 color;
out vec4 Color;

void main() 
{
    Color = color;
}

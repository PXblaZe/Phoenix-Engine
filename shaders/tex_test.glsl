// name = v, type = vertex

#version 450 core
layout(location=0) in vec2 pos;
layout(location=0) in vec2 intex;

out vec2 outex;

void main()
{
    outex = intex;
    gl_Position = vec4(pos, 0, 1);
}


// name = f, type = fragment

#version 450 core

in vec2 outex;
out vec4 outColor;

uniform sampler2D samTex;

void main()
{
    outColor = texture(samTex, outex);
}

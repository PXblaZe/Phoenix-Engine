// name = ver, type = vertex

#version 450 core

layout(location=0) in vec3 pos;
layout(location=1) in vec2 intex;
uniform float alpha;
out float ch_a;
out vec2 tex;
out vec3 col;

void main()
{
    tex = intex;
    ch_a = alpha;
    col = exp(pos-1);
    gl_Position = vec4(pos, 1);
}


// name = frag, type = fragment

#version 450 core

#pragma pxe_include_shader "utils.glsl"

out vec4 color;
in vec3 col;
uniform vec3 c[3];
in float ch_a;
in vec2 tex;

uniform sampler2D theTex;

void main()
{
    // color = vec4(abs(cross(col, c)), 1);
    // color = vec4(c[0].g, c[1].r, c[2].r, 1);
    // color = vec4(pow(cross(col, vec3(c[0].b, c[1].b, c[2].b)), vec3(2)), ch_a);
    color = magic(col, vec3(c[0].b, c[1].b, c[2].b), ch_a);
    color = mix(texture(theTex, tex), color, .7);
}

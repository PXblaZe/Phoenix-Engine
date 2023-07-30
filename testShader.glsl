// name = ver, type = vertex

#version 450 

layout(location=0) in vec3 pos;

out vec3 col;

void main()
{
    col = exp(pos-1);
    gl_Position = vec4(pos, 1);
}


// name = frag, type = fragment

#version 450 

out vec4 color;
in vec3 col;
uniform vec4 c;

void main()
{
    color = vec4(col, 1);
}


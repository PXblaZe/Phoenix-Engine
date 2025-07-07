vec4 magic(vec3 a, vec3 b, float c)
{
    vec4 val = vec4(pow(cross(a, b), vec3(2)), c);
    return val;
}

#ifndef __PXE_PHONG_MODEL__
#define __PXE_PHONG_MODEL__

#pragma pxe_include_shader "ambientlighting.glsl"
#pragma pxe_include_shader "diffuselighting.glsl"
#pragma pxe_include_shader "specularlighting.glsl"

struct PhongModel {
    AmbientLight ambient;
    DiffuseLight diffuse;
    SpecularLight specular;
};

#endif

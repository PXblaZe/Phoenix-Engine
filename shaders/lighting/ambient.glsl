#ifndef PXE_AMBIENT_LIGHT
#define PXE_AMBIENT_LIGHT

struct AmbientLight {
    vec3 color;
    float intensity;
};

vec4 getAmbient(AmbientLight ambient) {
    return vec4(ambient.color * ambient.intensity, 1.0);
} 

#endif

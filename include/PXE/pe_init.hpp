#pragma once

#include <omp.h>
#include <csignal>

#if defined(__ARM_ARCH_6__) || defined(__RASPBERRYPI__)
    #include <GLES2/gl2.h>
#elif defined(__APPLE__)
    #include <OpenGL/gl.h>
#elif defined(__ANDROID__) || defined(ANDROID)
    #include <GLES3/gl32.h>
#else
    #include <GL/glew.h>
#endif


#include <GLFW/glfw3.h>


#define PXassert(x) if(!(x)) std::raise(SIGINT);
#ifdef PX_DEBUG 
    #define pxcall(x) PXclearGLERlogs(); \
    x; \
    PXassert(disGLERlog(#x, __FILE__, __LINE__)) 
#else 
    #define PXcall(...) __VA_ARGS__
#endif

void PXclearGLERlogs();

bool PXdisGLERlog(const char* funname, const char* filename, size_t line);
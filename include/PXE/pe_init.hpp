#pragma once

#include <omp.h>
#include <csignal>

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#elif defined(__ANDROID__) || defined(ANDROID)
    #include <GLES3/gl32.h>
#else
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>



#define PXassert(x) if(!(x)) std::raise(SIGINT);
#ifdef PX_DEBUG 
    #define PXcall(x) PXclearGLERlogs(); \
    x; \
    PXassert(PXdisGLERlog(#x, __FILE__, __LINE__)) 
#else 
    #define PXcall(...) __VA_ARGS__
#endif

template<typename _Tp>
struct GLtype {
    static_assert(sizeof(_Tp)==0, "Unsupported type for GLtype.");
};

template<> struct GLtype<signed char> {
    using type = GLchar;
    static constexpr unsigned int enumval = GL_BYTE;
};

template<> struct GLtype<unsigned char> {
    using type = GLubyte;
    static constexpr unsigned int enumval = GL_UNSIGNED_BYTE;
};

template<> struct GLtype<bool> {
    using type = GLboolean;
    static  constexpr unsigned int enumval = GL_BOOL;
};

template<> struct GLtype<signed int> {
    using type = GLint;
    static constexpr unsigned int enumval = GL_INT;
};

template<> struct GLtype<unsigned int> {
    using type = GLuint;
    static constexpr unsigned int enumval = GL_UNSIGNED_INT;
};

template<> struct GLtype<signed short> {
    using type = GLshort;
    static constexpr unsigned int enumval = GL_SHORT;
};

template<> struct GLtype<unsigned short> {
    using type = GLushort;
    static constexpr unsigned int enumval = GL_UNSIGNED_SHORT;
};

template<> struct GLtype<float> {
    using type = GLfloat;
    static constexpr unsigned int enumval = GL_FLOAT;
};

template<> struct GLtype<double> {
    using type = GLdouble;
    static constexpr unsigned int enumval = GL_DOUBLE;
};

template<> struct GLtype<unsigned long> {
    using type = GLulong;
    static constexpr unsigned int enumval = GL_NONE;
};
template<> struct GLtype<void> {
    using type = GLvoid;
    static constexpr unsigned int enumval = GL_NONE;
};

const char* to_hex(unsigned int num);

void PXclearGLERlogs();

bool PXdisGLERlog(const char* funname, const char* filename, size_t line);
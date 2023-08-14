#pragma once

#include <vector>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "__pe_win32.hpp"

namespace px
{
    
class ShaderCode {
    struct ShaderFormat
    {
        const unsigned int type;
        const char* const codestr = nullptr;
    };
    
    std::unordered_map<std::string, struct ShaderFormat> shadermap;
public:
    ShaderCode(const char* filepath, const char* const delim = ",", const char* name = "name", const char* shaderType = "shader");
    inline const char* const parseCode(const char* const& name) const
    {
        return shadermap.find(name) == shadermap.end() ? nullptr : this->shadermap.at(name).codestr;
    }
    inline const unsigned int parseType(const char* const& name) const
    {
        return shadermap.find(name) == shadermap.end() ? 0 : this->shadermap.at(name).type;
    }
    inline void remove(const char* const& name)
    {
        auto it = shadermap.find(name);
        if (it != shadermap.end()) {
            free((char*)it->second.codestr);
            shadermap.erase(it);
        }
    }
    const std::vector<std::string> getNames() const;
    ~ShaderCode();
};


class ShaderProgram {

    unsigned int programid;
    unsigned int curshaders[5] = {};

    struct UniAttriFormat {
        unsigned int index; // location of the GLSL variable
        std::string name;
        unsigned int type; // type of the GLSL variable (float, vec3 or mat4, etc)
        int arrayLength; // size of the GLSL variable (if varable is not an array arrayLength = 1)
    };

    template <typename... Args>
    struct AreAllSameAndAllowedTypes {
        static constexpr bool value =
            std::conjunction<std::is_same<Args, typename std::common_type<Args...>::type>...>::value &&
            std::conjunction<std::__is_one_of<Args, signed int, unsigned int, float, double>...>::value;
    };

    template<typename _T, typename _F, typename... _Nxt>
    struct is_same_as_first: std::is_same<_T, _F> {};

    template <typename First, typename... Next>
    inline constexpr const First* firstPointer(unsigned char v, int n, First first, Next... args) const
    { 
        First* pData = (First*)malloc(v*n*sizeof(First));
        size_t i = 1; pData[0] = first;
        ((pData[i++] = args), ...);
        return pData;
    }    

    template<typename _Tp, int (&_Lf)(unsigned int, const char*), void (&_Uf)(int, const _Tp*, unsigned char, int)>
    void setUfm(const char* name, const _Tp* value, unsigned char _i, int _n) const
    {
        int myUniformLoc = _Lf(programid, name);

        if (myUniformLoc == -1) {
            throw std::invalid_argument(std::string(std::to_string(__LINE__-5)+"| "+__FILE__+" error: uniform variable `"+name+"` not found in the shader program (programID: "+std::to_string(programid)+")\n").c_str());
        }

        _Uf(myUniformLoc, value, _i, _n);
    }

    void __setUniform(const char* name, const signed int* value, unsigned char veci, int n) const;
    void __setUniform(const char* name, const unsigned int* value, unsigned char veci, int n) const;
    void __setUniform(const char* name, const float* value,  unsigned char veci, int n) const;
    void __setUniform(const char* name, const double* value, unsigned char veci, int n) const;

public:

    enum ShaderType {
        None = 0,
        VERTEX_SHADER = 1,
        FRAGMENT_SHADER = 2,
        GEOMETRY_SHADER = 4, 
        TESS_CONTROL_SHADER = 8,
        TESS_EVALUATION_SHADER = 16,
    };
    
    ShaderType ActiveShaders;

    typedef struct UniAttriFormat UniAttriFormat;

    ShaderProgram();

    void use() const;

    static void idle();

    void createProgram(
        const char* const& vertexShaderCode = nullptr, 
        const char* const& fragmentShaderCode = nullptr, 
        const char* const& geometryShaderCode = nullptr,
        const char* const& tess_controlShaderCode = nullptr,
        const char* const& tess_evalutionShaderCode = nullptr
    );

    template<
    unsigned char vec = 1, int len = 1, typename... _Tps, typename = std::enable_if_t<AreAllSameAndAllowedTypes<_Tps...>::value>>
    inline void setUniform(const char* name, _Tps... values) const
    {
        auto* pData = firstPointer(vec, len, values...);
        this->__setUniform(name, pData, vec, len);
        free((void*)pData);
    }

    template<
    unsigned char vec = 1, int len = 1, typename _Tp, typename = std::enable_if_t<std::__is_one_of<_Tp, signed int, unsigned int, float, double>::value>>
    inline void setUniform(const char* name, const _Tp* values) const
    {
        this->__setUniform(name, values, vec, len);
    }

    void getUniform(const char* name, signed int* data) const;
    void getUniform(const char* name, unsigned int* data) const;
    void getUniform(const char* name, float* data) const;
    void getUniform(const char* name, double* data) const;

    unsigned int getShaderType(ShaderType type) const;

    const std::vector<UniAttriFormat> getUniformsList() const;

    const std::vector<UniAttriFormat> getAttributesList() const;

    inline unsigned int getProgramID() const noexcept { return programid; }

    void cleanShaders(unsigned char maskShaderType);

};

} // namespace px
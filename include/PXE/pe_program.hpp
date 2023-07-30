#pragma once

#include <vector>
#include <string>
#include <unordered_map>


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

public:

    enum ShaderType {
        VERTEX_SHADER = 1,
        FRAGMENT_SHADER = 2,
        GEOMETRY_SHADER = 4, 
        TESS_CONTROL_SHADER = 8,
        TESS_EVALUATION_SHADER = 16,
        ALL = 0x11111
    };
    
    ShaderProgram();
    static void use(const ShaderProgram& program);

    void createProgram(
        const char* const& vertexShaderCode = nullptr, 
        const char* const& fragmentShaderCode = nullptr, 
        const char* const& geometryShaderCode = nullptr,
        const char* const& tess_controlShaderCode = nullptr,
        const char* const& tess_evalutionShaderCode = nullptr
    );

    unsigned int getShaderID(ShaderType type) const;

    template<typename _Tp, int (&_Lf)(unsigned int, const char*), void (&_Uf)(int, _Tp)>
    inline void setUniform(const char* const& name, const _Tp& value) const
    {
        int myUniformLoc = _Lf(programid, name);

        if (myUniformLoc == -1) {
            std::__throw_invalid_argument(std::string(std::to_string(__LINE__-5)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        _Uf(myUniformLoc, value);
    }

    template<typename _Tp, int (&_Lf)(unsigned int, const char*), void (&_Uf)(int, const _Tp*, unsigned char, int)>
    inline void setUniform(const char* const& name, const _Tp* value, unsigned char _i, int _n) const
    {
        int myUniformLoc = _Lf(programid, name);

        if (myUniformLoc == -1) {
            std::__throw_invalid_argument(std::string(std::to_string(__LINE__-5)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        _Uf(myUniformLoc, value, _i, _n);
    }

    void setUniform(const char* const& name, const signed int& value) const;
    void setUniform(const char* const& name, const signed int* value, unsigned int n) const;
    void setUniform(const char* const& name, const signed int* value, unsigned char veci, unsigned int n = 1) const;
    void setUniform(const char* const& name, const unsigned int& value) const;
    void setUniform(const char* const& name, const unsigned int* value, unsigned int n) const;
    void setUniform(const char* const& name, const unsigned int* value, unsigned char veci, unsigned int n = 1) const;
    void setUniform(const char* const& name, const float& value) const;
    void setUniform(const char* const& name, const float* value, unsigned int n) const;
    void setUniform(const char* const& name, const float* value,  unsigned char veci, unsigned int n = 1) const;
    void setUniform(const char* const& name, const double& value) const;
    void setUniform(const char* const& name, const double* value, unsigned int n) const;
    void setUniform(const char* const& name, const double* value, unsigned char veci, unsigned int n = 1) const;

    void getUniform(const char* const& name, signed int* data) const;
    void getUniform(const char* const& name, unsigned int* data) const;
    void getUniform(const char* const& name, float* data) const;
    void getUniform(const char* const& name, double* data) const;

    void cleanShaders(unsigned char maskShaderType);

};


} // namespace px


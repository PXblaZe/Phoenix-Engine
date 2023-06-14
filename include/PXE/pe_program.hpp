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
        const char* const codestr;
    };
    
    std::unordered_map<std::string, struct ShaderFormat> shadermap;
public:
    ShaderCode(const char* filepath, const char* const delim = ",", const char* name = "name", const char* shaderType = "shader");
    inline const char* const parseCode(const char* const& name) const {
        return shadermap.find(name) == shadermap.end() ? nullptr : this->shadermap.at(name).codestr;
    }
    inline const unsigned int parseType(const char* const& name) const {
        return shadermap.find(name) == shadermap.end() ? 0 : this->shadermap.at(name).type;
    }
    const std::vector<std::string> getNames() const;
    ~ShaderCode();
};

class ShaderProgram {

    unsigned int programid;
    std::unordered_map<std::string, std::pair<unsigned int, bool>> shaderstate;

public:

    ShaderProgram();
    void compileShader(const ShaderCode& scode, const char* const& name = nullptr);
    void deleteShader(const ShaderCode& scode, const char* const& name = nullptr);
    void attachShader(const char* const& name = nullptr);
    void detachShader(const char* const& name = nullptr);
    // void create
};


} // namespace px


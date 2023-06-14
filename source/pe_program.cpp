#include <stack>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "PXE/pe_init.hpp"
#include "PXE/pe_program.hpp"

#if defined(_WIN32) || defined(_WIN64)
    #define strdupa(str) strcpy((char*)alloca(strlen(str) + 1), str)
    #define strdup _strdup
#endif

GLtype<unsigned int>::type cplShader(unsigned int type, const char* const GLScode, char** message) {
    GLtype<unsigned int>::type shid = glCreateShader(type);
    glShaderSource(shid, 1, &GLScode, nullptr);
    glCompileShader(shid);

    GLint res;
    glGetShaderiv(shid, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE) {
        GLint mlen;
        glGetShaderiv(shid, GL_INFO_LOG_LENGTH, &mlen);
        char* msg = (char*)malloc(mlen);
        glGetShaderInfoLog(shid, mlen, &mlen, msg);
        message = &msg;
        return 0;
    }
    
    return shid;
}


namespace px
{
    ShaderCode::ShaderCode(const char* filepath, const char* delim, const char* name, const char* shaderType) 
    {        
        size_t len = 0, clen = 0;
        FILE* pshc = fopen(filepath, "r");
        if (pshc == nullptr) perror("Unable to open file");

        unsigned int tv;
        char* line = nullptr, *target = nullptr, *cstr = nullptr;

        while (getline(&line, &len, pshc) != -1)
        {
            if (!memcmp(line, "//", 2)) {

                if (cstr != nullptr) {
                    this->shadermap.insert(std::make_pair<std::string, struct ShaderCode::ShaderFormat>(target, {tv, cstr}));
                    cstr = nullptr;
                }
                
                line[strlen(line)-1] = '\0';
                line += 2;

                char* psv = line;
                std::stack<std::string> tkns;

                while (true) {
                    char* tkn = strtok_r(nullptr, delim, &psv);
                    if (tkn == nullptr) break;
                    tkns.push(tkn);
                }
                while (!tkns.empty()) {
                    tkns.top().erase(
                        std::remove_if(
                            tkns.top().begin(), 
                            tkns.top().end(),
                            [](unsigned char c) { return std::isspace(c); }
                        ),
                        tkns.top().end()
                    );
                    
                    psv = strdupa(tkns.top().c_str());

                    char* tknn = strtok_r(nullptr, "=", &psv);
                    char* tknt = strtok_r(nullptr, "=", &psv);
                    if (!memcmp(tknn, name, strlen(name)))  
                        target = strdup(tknt);
                    else if (!memcmp(tknn, shaderType, strlen(shaderType))) {
                        std::for_each(
                            tknt, tknt+strlen(tknt), 
                            [](char& c) {c = std::toupper(c);}
                        );
                        if (!memcmp(tknt, "VERTEX", 7))
                            tv = GL_VERTEX_SHADER;
                        else if (!memcmp(tknt, "FRAGMENT", 9))
                            tv = GL_FRAGMENT_SHADER;
                        else if (!memcmp(tknt, "GEOMETRY", 9))
                            tv = GL_GEOMETRY_SHADER;
                        else if (!memcmp(tknt, "TESS_CONTROL", 13))
                            tv = GL_TESS_CONTROL_SHADER;
                        else if (!memcmp(tknt, "TESS_EVALUATION", 16))
                            tv = GL_TESS_EVALUATION_SHADER;
                    }
                    tkns.pop();
                }
                line -= 2;
                clen = 0;
            }
            else {
                size_t llen = strlen(line);
                cstr = (char*) realloc(cstr, cstr == nullptr? llen+1: clen+llen+1);
                if (cstr == nullptr) std::__throw_bad_alloc();
                memcpy(cstr+clen, line, llen+1);
                clen += llen;
                cstr[clen] = '\0';
            }
            

        }
        if (cstr != nullptr) this->shadermap.insert(std::make_pair<std::string, struct ShaderCode::ShaderFormat>(target, {tv, cstr}));
        free(line);
        fclose(pshc);
    }

    const std::vector<std::string> ShaderCode::getNames() const
    {
        std::vector<std::string> nvec(shadermap.size());

        std::transform(shadermap.begin(), shadermap.end(), std::back_inserter(nvec),
            [](const std::pair<const std::string, ShaderCode::ShaderFormat>& pair) {
                return pair.first;
            }
        );

        return nvec;
    }
    
    ShaderCode::~ShaderCode()
    {
        auto psm = this->shadermap.begin();
        while (psm!=this->shadermap.end()) free((char*)psm->second.codestr), psm++;
        this->shadermap.clear();
    }


    ShaderProgram::ShaderProgram() {
        this->programid = glCreateProgram();
    }

    void ShaderProgram::compileShader(const ShaderCode& scode, const char* const& name)
    {
        char* msg;
        GLtype<unsigned int>::type shaderid = 0;
        if (name != nullptr && shaderstate.find(name) == shaderstate.end()) {
            shaderid = cplShader(scode.parseType(name), scode.parseCode(name), &msg);
            if (shaderid) shaderstate.insert(std::make_pair(name, std::make_pair(shaderid, false)));
            else std::__throw_runtime_error(std::string(std::to_string(__LINE__-1)+"| "+__FILE__+" error: failed to compile "+name+" shader (type: 0x"+to_hex(scode.parseType(name))+")\n"+msg).c_str());
            return;
        }

        for (std::string n: scode.getNames()) {
            shaderid = cplShader(scode.parseType(n.c_str()), scode.parseCode(n.c_str()), &msg);
            if (shaderid) shaderstate.insert(std::make_pair(name, std::make_pair(shaderid, false)));
            else std::__throw_runtime_error(std::string(std::to_string(__LINE__-1)+"| "+__FILE__+" error: failed to compile "+n+" shader (type: 0x"+to_hex(scode.parseType(n.c_str()))+")\n"+msg).c_str());
        }
        
    }

    void ShaderProgram::deleteShader(const ShaderCode &scode, const char* const& name)
    {

    }

    void ShaderProgram::attachShader(const char *const &name)
    {

    }

    void ShaderProgram::detachShader(const char *const &name)
    {

    }

} // namespace px
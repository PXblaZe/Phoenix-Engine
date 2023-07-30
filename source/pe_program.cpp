#include <stack>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "PXE/pe_init.hpp"
#include "PXE/pe_program.hpp"


#if defined(_WIN32) || defined(_WIN64)
    #define strdupa(str) strcpy((char*)alloca(strlen(str) + 1), (str))
    #define strdup(str) _strdup(str)
#endif

GLtype<unsigned int>::type cplShader(unsigned int type, const char* const GLScode, char* &message) {
    GLtype<unsigned int>::type shid = PXcall(glCreateShader(type));
    PXcall(glShaderSource(shid, 1, &GLScode, nullptr));
    PXcall(glCompileShader(shid));

    GLint res;
    PXcall(glGetShaderiv(shid, GL_COMPILE_STATUS, &res));
    if (res == GL_FALSE) {
        GLint mlen;
        PXcall(glGetShaderiv(shid, GL_INFO_LOG_LENGTH, &mlen));
        message = (char*)realloc(message, mlen);
        PXassert(message != nullptr);
        PXcall(glGetShaderInfoLog(shid, mlen, &mlen, message));
        return 0;
    }
    
    return shid;
}


//    -- glGetUniformLocation() Wrappers --
int locW(unsigned int a, const char* p) {return PXcall(glGetUniformLocation(a, p));}


//    -- glUniform*() Wrappers --

inline void uniiinW(int a, const signed int* b, unsigned char c, int d) {
    if (c == 1) PXcall(glUniform1iv(a, d, b));
    else if (c == 2) PXcall(glUniform2iv(a, d, b));
    else if (c == 3) PXcall(glUniform3iv(a, d, b));
    else if (c == 4) PXcall(glUniform4iv(a, d, b));
}
inline void uniiuinW(int a, const unsigned int* b, unsigned char c, int d) {
    if (c == 1) PXcall(glUniform1uiv(a, d, b));
    else if (c == 2) PXcall(glUniform2uiv(a, d, b));
    else if (c == 3) PXcall(glUniform3uiv(a, d, b));
    else if (c == 4) PXcall(glUniform4uiv(a, d, b));
}
inline void uniifnW(int a, const float* b, unsigned char c, int d) {
    if (c == 1) PXcall(glUniform1fv(a, d, b));
    else if (c == 2) PXcall(glUniform2fv(a, d, b));
    else if (c == 3) PXcall(glUniform3fv(a, d, b));
    else if (c == 4) PXcall(glUniform4fv(a, d, b));
}
inline void uniidnW(int a, const double* b, unsigned char c, int d) {
    if (c == 1) PXcall(glUniform1dv(a, d, b));
    else if (c == 2) glUniform2dv(a, d, b);
    else if (c == 3) glUniform3dv(a, d, b);
    else if (c == 4) glUniform4dv(a, d, b);
}
inline void uniiW(int a, signed int b) { glUniform1i(a, b); }
inline void uniuiW(int a, unsigned int b) { glUniform1ui(a, b); }
inline void unifW(int a, float b) { glUniform1f(a, b); }
inline void unidW(int a, double b) { glUniform1d(a, b); }


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
                        #if defined(GL_ARB_geometry_shader4) || defined(GL_EXT_geometry_shader4) || defined(GL_EXT_geometry_shader) || defined(GL_VERSION_3_2)|| defined(GL_ES_VESRION_3_2)
                        else if (!memcmp(tknt, "GEOMETRY", 9))
                            tv = GL_GEOMETRY_SHADER;
                        #endif
                        #if defined(GL_ARB_tessellation_shader) || defined(GL_EXT_tessellation_shader) || defined(GL_VERSION_4_0)|| defined(GL_ES_VESRION_3_2)
                        else if (!memcmp(tknt, "TESS_CONTROL", 13))
                            tv = GL_TESS_CONTROL_SHADER;
                        else if (!memcmp(tknt, "TESS_EVALUATION", 16))
                            tv = GL_TESS_EVALUATION_SHADER;
                        #endif
                        
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


    ShaderProgram::ShaderProgram(): programid(glCreateProgram()) {}

    void ShaderProgram::use(const ShaderProgram& program) {
        glUseProgram(program.programid);
    }

    void ShaderProgram::createProgram(
        const char* const& vertexShaderCode, 
        const char* const& fragmentShaderCode, 
        const char* const& geometryShaderCode,
        const char* const& tess_controlShaderCode,
        const char* const& tess_evalutionShaderCode
    ) {
        char* msg = nullptr;
        GLtype<unsigned int>::type shaderid = 0;

        if (vertexShaderCode != nullptr)
        {
            shaderid = cplShader(GL_VERTEX_SHADER, vertexShaderCode, msg);
            if (shaderid) this->curshaders[0] = shaderid;
            else std::__throw_runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile vertex shader (type: 0x"+to_hex(GL_VERTEX_SHADER)+")\n"+msg).c_str());
        }
        if (fragmentShaderCode != nullptr)
        {
            shaderid = cplShader(GL_FRAGMENT_SHADER, fragmentShaderCode, msg);
            if (shaderid) this->curshaders[1] = shaderid;
            else std::__throw_runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile fragment shader (type: 0x"+to_hex(GL_FRAGMENT_SHADER)+")\n"+msg).c_str());
        }
        if (geometryShaderCode != nullptr) 
        {
            shaderid = cplShader(GL_GEOMETRY_SHADER, geometryShaderCode, msg);
            if (shaderid) this->curshaders[2] = shaderid;
            else std::__throw_runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile geometry shader (type: 0x"+to_hex(GL_GEOMETRY_SHADER)+")\n"+msg).c_str());
        }
        if (tess_controlShaderCode != nullptr)
        {
            shaderid = cplShader(GL_TESS_CONTROL_SHADER, tess_controlShaderCode, msg);
            if (shaderid) this->curshaders[3] = shaderid;
            else std::__throw_runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile tessellation control shader (type: 0x"+to_hex(GL_TESS_CONTROL_SHADER)+")\n"+msg).c_str());
        }
        if (tess_evalutionShaderCode != nullptr)
        {
            shaderid = cplShader(GL_TESS_EVALUATION_SHADER, tess_evalutionShaderCode, msg);
            if (shaderid) this->curshaders[4] = shaderid;
            else std::__throw_runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile tessellation evaluation shader (type: 0x"+to_hex(GL_TESS_EVALUATION_SHADER)+")\n"+msg).c_str());
        }

        for (int i = 0; i < 5; i++) if (this->curshaders[i]) glAttachShader(this->programid, this->curshaders[i]);

        GLtype<int>::type status;

        glLinkProgram(this->programid);
        glGetProgramiv(this->programid, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLtype<int>::type logLength;
            glGetProgramiv(this->programid, GL_INFO_LOG_LENGTH, &logLength);
            msg = (char*)realloc(msg, logLength);
            glGetProgramInfoLog(this->programid, logLength, nullptr, msg);
            std::__throw_runtime_error(std::string(std::to_string(__LINE__-7)+"| "+__FILE__+" error: failed to link shader program (programID: "+std::to_string(this->programid)+")\n"+msg).c_str());
        }


        glValidateProgram(this->programid);
        glGetProgramiv(this->programid, GL_VALIDATE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint logLength;
            glGetProgramiv(this->programid, GL_INFO_LOG_LENGTH, &logLength);
            msg = (char*)realloc(msg, logLength);
            glGetProgramInfoLog(this->programid, logLength, nullptr, msg);
            std::__throw_runtime_error(std::string(std::to_string(__LINE__-7)+"| "+__FILE__+" error: failed to validate shader program (programID: "+std::to_string(this->programid)+")\n"+msg).c_str());
        }

        if (msg != nullptr) free(msg);
    }

    unsigned int ShaderProgram::getShaderID(ShaderType type) const
    {
        switch (type)
        {
            case ShaderProgram::VERTEX_SHADER: return this->curshaders[0];
            case ShaderProgram::FRAGMENT_SHADER: return this->curshaders[1];
            case ShaderProgram::GEOMETRY_SHADER: return this->curshaders[2];
            case ShaderProgram::TESS_CONTROL_SHADER: return this->curshaders[3];
            case ShaderProgram::TESS_EVALUATION_SHADER: return this->curshaders[4];
            default: return 0;
        }
    }

    void ShaderProgram::setUniform(const char* const& name, const signed int& value) const
    {
        this->setUniform<signed int, locW, uniiW>(name, value);
    }

    void ShaderProgram::setUniform(const char* const& name, const unsigned int& value) const
    {
        this->setUniform<unsigned int, locW, uniuiW>(name, value);
    }

    void ShaderProgram::setUniform(const char* const& name, const float& value) const
    {
        this->setUniform<float, locW, unifW>(name, value);
    }

    void ShaderProgram::setUniform(const char* const& name, const double& value) const
    {
        this->setUniform<double, locW, unidW>(name, value);
    }

    void ShaderProgram::setUniform(const char* const& name, const signed int* value, unsigned int n) const
    {
        this->setUniform<signed int, locW, uniiinW>(name, value, 1, n);
    }
    void ShaderProgram::setUniform(const char* const& name, const unsigned int* value, unsigned int n) const
    {
        this->setUniform<unsigned int, locW, uniiuinW>(name, value, 1, n);
    }
    void ShaderProgram::setUniform(const char* const& name, const float* value, unsigned int n) const
    {
        this->setUniform<float, locW, uniifnW>(name, value, 1, n);
    }
    void ShaderProgram::setUniform(const char* const& name, const double* value, unsigned int n) const
    {
        this->setUniform<double, locW, uniidnW>(name, value, 1, n);
    }

    void ShaderProgram::setUniform(const char* const& name, const signed int* value, unsigned char veci, unsigned int n) const
    {
        this->setUniform<signed int, locW, uniiinW>(name, value, veci, n);
    }

    void ShaderProgram::setUniform(const char* const& name, const unsigned int* value, unsigned char veci, unsigned int n) const
    {
        this->setUniform<unsigned int, locW, uniiuinW>(name, value, veci, n);
    }

    void ShaderProgram::setUniform(const char* const& name, const float* value, unsigned char veci, unsigned int n) const
    {
        this->setUniform<float, locW, uniifnW>(name, value, veci, n);
    }

    void ShaderProgram::setUniform(const char* const& name, const double* value, unsigned char veci, unsigned int n) const
    {
        this->setUniform<double, locW, uniidnW>(name, value, veci, n);
    }

    void ShaderProgram::getUniform(const char* const& name, signed int* data) const
    {
        int loc = glGetUniformLocation(this->programid, name);
        if (loc == -1) {
            std::__throw_invalid_argument(std::string(std::to_string(__LINE__-4)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        glGetUniformiv(this->programid, loc, data);
    }

    void ShaderProgram::getUniform(const char* const& name, unsigned int* data) const
    {
        int loc = glGetUniformLocation(this->programid, name);
        if (loc == -1) {
            std::__throw_invalid_argument(std::string(std::to_string(__LINE__-4)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        glGetUniformuiv(this->programid, loc, data);
    }

    void ShaderProgram::getUniform(const char* const& name, float* data) const
    {
        int loc = glGetUniformLocation(this->programid, name);
        if (loc == -1) {
            std::__throw_invalid_argument(std::string(std::to_string(__LINE__-4)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        glGetUniformfv(this->programid, loc, data);
    }

    void ShaderProgram::getUniform(const char* const& name, double* data) const
    {
        int loc = glGetUniformLocation(this->programid, name);
        if (loc == -1) {
            std::__throw_invalid_argument(std::string(std::to_string(__LINE__-4)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        glGetUniformdv(this->programid, loc, data);
    }

    void ShaderProgram::cleanShaders(unsigned char maskShaderType)
    {
        while (maskShaderType) {
            unsigned char type = maskShaderType & -maskShaderType;
            switch (type)
            {
            case ShaderProgram::VERTEX_SHADER:
                glDeleteShader(this->curshaders[0]), this->curshaders[0] = 0;
                break;
            case ShaderProgram::FRAGMENT_SHADER: 
                glDeleteShader(this->curshaders[1]), this->curshaders[1] = 0;
                break;
            case ShaderProgram::GEOMETRY_SHADER:
                glDeleteShader(this->curshaders[2]), this->curshaders[2] = 0;
                break;
            case ShaderProgram::TESS_CONTROL_SHADER:
                glDeleteShader(this->curshaders[3]), this->curshaders[3] = 0;
                break;
            case ShaderProgram::TESS_EVALUATION_SHADER:
                glDeleteShader(this->curshaders[4]), this->curshaders[4] = 0;
                break;
            }
            maskShaderType ^= type;
        }
    }

} // namespace px
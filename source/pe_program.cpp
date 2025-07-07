#include <stack>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <algorithm>
#include <filesystem>

#include "PXE/pe_init.hpp"
#include "PXE/pe_program.hpp"


GLtype<unsigned int>::type cplShader(GLtype<unsigned int>::type type, const char* const GLScode, char* &message) {
    PXcall(GLtype<unsigned int>::type shid = glCreateShader(type));
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
GLtype<int>::type locW(GLtype<unsigned int>::type a, const GLchar* p) {
    PXcall(GLtype<int>::type loc = glGetUniformLocation(a, p));
    return loc;
}

//    -- glUniform*v() Wrappers --

void uniiinW(int a, const signed int* b, unsigned char i, int n) {
    if (i == 1) {PXcall(glUniform1iv(a, n, b));}
    else if (i == 2) {PXcall(glUniform2iv(a, n, b));}
    else if (i == 3) {PXcall(glUniform3iv(a, n, b));}
    else if (i == 4) {PXcall(glUniform4iv(a, n, b));}
}

void uniifnW(int a, const float* b, unsigned char i, int n) {
    if (i == 1) {PXcall(glUniform1fv(a, n, b));}
    else if (i == 2) {PXcall(glUniform2fv(a, n, b));}
    else if (i == 3) {PXcall(glUniform3fv(a, n, b));}
    else if (i == 4) {PXcall(glUniform4fv(a, n, b));}
}

#if defined(GL_VERSION_3_0) || defined(GL_ES_VERSION_3_0)
void uniiuinW(int a, const unsigned int* b, unsigned char i, int n) {
    if (i == 1) {PXcall(glUniform1uiv(a, n, b));}
    else if (i == 2) {PXcall(glUniform2uiv(a, n, b));}
    else if (i == 3) {PXcall(glUniform3uiv(a, n, b));}
    else if (i == 4) {PXcall(glUniform4uiv(a, n, b));}
}
#endif

#if defined(GL_VERSION_2_0) || defined(GL_ES_VERSION_3_0) || defined(GL_ARB_gpu_shader_fp64)
void uniidnW(int a, const double* b, unsigned char i, int n) {
    if (i == 1) {PXcall(glUniform1dv(a, n, b));}
    else if (i == 2) {PXcall(glUniform2dv(a, n, b));}
    else if (i == 3) {PXcall(glUniform3dv(a, n, b));}
    else if (i == 4) {PXcall(glUniform4dv(a, n, b));}
}
#endif

void unimijfnW(int a, const float* b, unsigned char i, unsigned char j, int n, bool t) {
    if (i == 2 && j == 2) {PXcall(glUniformMatrix2fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
    else if (i == 3 && j == 3) {PXcall(glUniformMatrix3fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
    else if (i == 4 && j == 4) {PXcall(glUniformMatrix4fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
#if defined(GL_VERSION_2_1) || defined(GL_ES_VERSION_3_1)
    else if (i == 2 && j == 3) {PXcall(glUniformMatrix2x3fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
    else if (i == 2 && j == 4) {PXcall(glUniformMatrix2x4fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
    else if (i == 3 && j == 2) {PXcall(glUniformMatrix3x2fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
    else if (i == 3 && j == 4) {PXcall(glUniformMatrix3x4fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
    else if (i == 4 && j == 2) {PXcall(glUniformMatrix4x2fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
    else if (i == 4 && j == 3) {PXcall(glUniformMatrix4x3fv(a, n, t ? GL_TRUE : GL_FALSE, b));}
#endif
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
                    if (!memcmp(tknn, name, strlen(name))) {
                        if (target) free(target), target = nullptr;
                        target = strdup(tknt);
                    }
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
                bool lineswitch = false;
                size_t llen = strlen(line);
                char* pragline = strstr(line, "#pragma"), *linetemp = nullptr;
                if (pragline != nullptr)
                {
                    char* plsvp = nullptr, *include_path;
                    pragline = strtok_r(pragline, " ", &plsvp);
                    pragline = strtok_r(nullptr, " ", &plsvp);
                    if (pragline == nullptr || strlen(pragline)!=18 || memcmp(pragline, "pxe_include_shader", 18)) 
                        throw std::runtime_error(std::string("Error: Invalid pragma in file ") + filepath);
                    include_path = strtok_r(nullptr, " ", &plsvp);
                    if (include_path == nullptr || (*include_path != '<' && *include_path != '\"'))
                        throw std::runtime_error(std::string("Error: Invalid path specification for pragma pxe_include_shader in file ") + filepath);
                    plsvp--; if (*plsvp == '\0' || *plsvp == '\n') plsvp--;
                    if ((*include_path == '<' && *plsvp != '>') || (*include_path == '\"' && *plsvp != '\"'))
                        throw std::runtime_error(std::string("Error: Invalid path specification for pragma pxe_include_shader in file ") + filepath);
                    *plsvp = '\0', include_path++;
                    
                    FILE* pINCPAT = fopen((std::filesystem::path(filepath).remove_filename()/include_path).string().c_str(), "r");
                    if (pINCPAT == nullptr) {
                        perror((std::string("Error: Invalid file path ") + include_path + " for pragma pxe_include_shader in file " + filepath).c_str());
                        exit(1);
                    }
                    if (fseek(pINCPAT, 0, SEEK_END) != 0) {
                        perror((std::string("Error: Failed in seeking to end of file ") + include_path + " for pragma pxe_include_shader in file " + filepath).c_str());
                        fclose(pINCPAT);
                        exit(1);
                    }
                    long szINCPAT = ftell(pINCPAT);
                    if (-1L == szINCPAT)
                    {
                        perror((std::string("Error: Failed in getting file size of ") + include_path + " for pragma pxe_include_shader in file " + filepath).c_str());
                        fclose(pINCPAT);
                        exit(1);
                    }
                    char* dataINCPAT = static_cast<char*>(malloc(szINCPAT+2));
                    if (dataINCPAT == nullptr) throw std::bad_alloc();
                    rewind(pINCPAT);
                    if (fread(dataINCPAT, 1, szINCPAT, pINCPAT) != szINCPAT) {
                        perror((std::string("Error: Failed in reading fiie ") + include_path + " for pragma pxe_include_shader in file " + filepath).c_str());
                        free(dataINCPAT);
                        fclose(pINCPAT);
                        exit(1);
                    }
                    linetemp = line;
                    lineswitch = true;
                    line = dataINCPAT;
                    line[szINCPAT] = '\n', line[szINCPAT+1] = '\0';
                    llen = static_cast<size_t>(szINCPAT+1);
                    fclose(pINCPAT);
                }
                cstr = (char*) realloc(cstr, cstr == nullptr? llen+1: clen+llen+1);
                if (cstr == nullptr) throw std::bad_alloc();
                memcpy(cstr+clen, line, llen+1);
                clen += llen;
                cstr[clen] = '\0';
                if (lineswitch) {
                    free(line);
                    line = linetemp;
                }
            }
            
        }
        if (cstr != nullptr) this->shadermap.insert(std::make_pair<std::string, struct ShaderCode::ShaderFormat>(target, {tv, cstr}));
        free(line);
        if (target) free(target);
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
    void ShaderProgram::idle() { PXcall(glUseProgram(0)); }

    void ShaderProgram::use() const { PXcall(glUseProgram(this->programid)); }

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
            else throw std::runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile vertex shader (type: 0x"+to_hex(GL_VERTEX_SHADER)+")\n"+msg).c_str());
        }
        if (fragmentShaderCode != nullptr)
        {
            shaderid = cplShader(GL_FRAGMENT_SHADER, fragmentShaderCode, msg);
            if (shaderid) this->curshaders[1] = shaderid;
            else throw std::runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile fragment shader (type: 0x"+to_hex(GL_FRAGMENT_SHADER)+")\n"+msg).c_str());
        }
        if (geometryShaderCode != nullptr) 
        {
            shaderid = cplShader(GL_GEOMETRY_SHADER, geometryShaderCode, msg);
            if (shaderid) this->curshaders[2] = shaderid;
            else throw std::runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile geometry shader (type: 0x"+to_hex(GL_GEOMETRY_SHADER)+")\n"+msg).c_str());
        }
        if (tess_controlShaderCode != nullptr)
        {
            shaderid = cplShader(GL_TESS_CONTROL_SHADER, tess_controlShaderCode, msg);
            if (shaderid) this->curshaders[3] = shaderid;
            else throw std::runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile tessellation control shader (type: 0x"+to_hex(GL_TESS_CONTROL_SHADER)+")\n"+msg).c_str());
        }
        if (tess_evalutionShaderCode != nullptr)
        {
            shaderid = cplShader(GL_TESS_EVALUATION_SHADER, tess_evalutionShaderCode, msg);
            if (shaderid) this->curshaders[4] = shaderid;
            else throw std::runtime_error(std::string(std::to_string(__LINE__-2)+"| "+__FILE__+" error: failed to compile tessellation evaluation shader (type: 0x"+to_hex(GL_TESS_EVALUATION_SHADER)+")\n"+msg).c_str());
        }

        for (int i = 0; i < 5; i++) if (this->curshaders[i]) {PXcall(glAttachShader(this->programid, this->curshaders[i]));}

        GLtype<int>::type status;

        PXcall(glLinkProgram(this->programid));
        PXcall(glGetProgramiv(this->programid, GL_LINK_STATUS, &status));
        if (status == GL_FALSE) {
            GLtype<int>::type logLength;
            PXcall(glGetProgramiv(this->programid, GL_INFO_LOG_LENGTH, &logLength));
            msg = (char*)realloc(msg, logLength);
            PXcall(glGetProgramInfoLog(this->programid, logLength, nullptr, msg));
            throw std::runtime_error(std::string(std::to_string(__LINE__-7)+"| "+__FILE__+" error: failed to link shader program (programID: "+std::to_string(this->programid)+")\n"+msg).c_str());
        }


        PXcall(glValidateProgram(this->programid));
        PXcall(glGetProgramiv(this->programid, GL_VALIDATE_STATUS, &status));
        if (status == GL_FALSE) {
            GLtype<int>::type logLength;
            PXcall(glGetProgramiv(this->programid, GL_INFO_LOG_LENGTH, &logLength));
            msg = (char*)realloc(msg, logLength);
            PXcall(glGetProgramInfoLog(this->programid, logLength, nullptr, msg));
            throw std::runtime_error(std::string(std::to_string(__LINE__-7)+"| "+__FILE__+" error: failed to validate shader program (programID: "+std::to_string(this->programid)+")\n"+msg).c_str());
        }
        
        if (msg != nullptr) free(msg);
    }

    unsigned int ShaderProgram::getShaderType(ShaderType type) const
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

    void ShaderProgram::__setUniform(const char* name, const signed int* value, unsigned char veci, int n) const
    {
        this->setUfm<signed int, locW, uniiinW>(name, value, veci, n);
    }

    void ShaderProgram::__setUniform(const char* name, const unsigned int* value, unsigned char veci, int n) const
    {
        this->setUfm<unsigned int, locW, uniiuinW>(name, value, veci, n);
    }

    void ShaderProgram::__setUniform(const char* name, const float* value, unsigned char veci, int n) const
    {
        this->setUfm<float, locW, uniifnW>(name, value, veci, n);
    }

    void ShaderProgram::__setUniform(const char* name, const double* value, unsigned char veci, int n) const
    {
        this->setUfm<double, locW, uniidnW>(name, value, veci, n);
    }

    void ShaderProgram::__setUniformMat(const char* name, const float* value, unsigned char r, unsigned char c, int n, bool tp) const
    {
        this->setUfmMat<locW, unimijfnW>(name, value, r, c, n, tp);
    }

    void ShaderProgram::getUniform(const char* name, signed int* data) const
    {
        PXcall(int loc = glGetUniformLocation(this->programid, name));
        if (loc == -1) {
            throw std::invalid_argument(std::string(std::to_string(__LINE__-4)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        PXcall(glGetUniformiv(this->programid, loc, data));
    }

    void ShaderProgram::getUniform(const char* name, unsigned int* data) const
    {
        PXcall(int loc = glGetUniformLocation(this->programid, name));
        if (loc == -1) {
            throw std::invalid_argument(std::string(std::to_string(__LINE__-4)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        PXcall(glGetUniformuiv(this->programid, loc, data));
    }

    void ShaderProgram::getUniform(const char* name, float* data) const
    {
        PXcall(int loc = glGetUniformLocation(this->programid, name));
        if (loc == -1) {
            throw std::invalid_argument(std::string(std::to_string(__LINE__-4)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        PXcall(glGetUniformfv(this->programid, loc, data));
    }

    void ShaderProgram::getUniform(const char* name, double* data) const
    {
        PXcall(int loc = glGetUniformLocation(this->programid, name));
        if (loc == -1) {
            throw std::invalid_argument(std::string(std::to_string(__LINE__-4)+"| "+__FILE__+" error: uniform variable `name` not found in the shader program (programID: "+std::to_string(this->programid)+")\n").c_str());
        }

        PXcall(glGetUniformdv(this->programid, loc, data));
    }

    const std::vector<ShaderProgram::UniAttriFormat> ShaderProgram::getUniformsList() const
    {
        GLtype<int>::type count = 0;
        PXcall(glGetProgramiv(this->programid, GL_ACTIVE_UNIFORMS, &count));

        std::vector<UniAttriFormat> list(count);

        GLtype<int>::type bufSize = 0; // maximum name length
        PXcall(glGetProgramiv(this->programid, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize)); bufSize++;

        GLchar* name = (GLchar*)alloca(bufSize); // variable name in GLSL

        for (GLtype<unsigned int>::type i = 0; UniAttriFormat& fmt : list)
        {   
            PXcall(glGetActiveUniform(this->programid, i, bufSize, nullptr, &fmt.arrayLength, &fmt.type, name));
            fmt.name = name, fmt.index = i++;
        }

        return list;
    }

    const std::vector<ShaderProgram::UniAttriFormat> ShaderProgram::getAttributesList() const
    {
        GLtype<int>::type count = 0;
        PXcall(glGetProgramiv(this->programid, GL_ACTIVE_ATTRIBUTES, &count));

        std::vector<UniAttriFormat> list(count);

        GLtype<int>::type bufSize = 0; // maximum name length
        PXcall(glGetProgramiv(this->programid, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize)); bufSize++;

        GLchar* name = (GLchar*)alloca(bufSize); // variable name in GLSL

        for (GLtype<unsigned int>::type i = 0; UniAttriFormat& fmt : list)
        {   
            PXcall(glGetActiveAttrib(this->programid, i, bufSize, nullptr, &fmt.arrayLength, &fmt.type, name));
            fmt.name = name, fmt.index = i++;
        }

        return list;
    }

    void ShaderProgram::cleanShaders(unsigned char maskShaderType)
    {
        while (maskShaderType) {
            unsigned char type = maskShaderType & -maskShaderType;
            switch (type)
            {
            case ShaderProgram::VERTEX_SHADER:
                PXcall(glDeleteShader(this->curshaders[0])); this->curshaders[0] = 0;
                break;
            case ShaderProgram::FRAGMENT_SHADER: 
                PXcall(glDeleteShader(this->curshaders[1])); this->curshaders[1] = 0;
                break;
            case ShaderProgram::GEOMETRY_SHADER:
                PXcall(glDeleteShader(this->curshaders[2])); this->curshaders[2] = 0;
                break;
            case ShaderProgram::TESS_CONTROL_SHADER:
                PXcall(glDeleteShader(this->curshaders[3])); this->curshaders[3] = 0;
                break;
            case ShaderProgram::TESS_EVALUATION_SHADER:
                PXcall(glDeleteShader(this->curshaders[4])); this->curshaders[4] = 0;
                break;
            }
            maskShaderType ^= type;
        }
    }

} // namespace px

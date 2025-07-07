#include <stdio.h>
#include <chrono>
#include <string.h>
#include <string_view>
#include <unordered_map>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "PXE/pe_mesh.hpp"

inline constexpr glm::vec3 crossProduct(const glm::vec3& a, const glm::vec3& b) noexcept {
    return glm::vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline constexpr float dotProduct(const glm::vec3& a, const glm::vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

constexpr void arangeCCW3p(glm::vec4 (&p3P)[3])
{
    glm::vec3 normal = (glm::vec3(p3P[1]) - glm::vec3(p3P[0]), glm::vec3(p3P[2]) - glm::vec3(p3P[0]));
    std::sort(p3P, p3P+3, [normal](const glm::vec3& a, const glm::vec3& b) {
        return dotProduct(normal, crossProduct(a, b)) > 0.0f;
    });
}

inline constexpr void fun_on_v(
    unsigned char* vd, unsigned char*, unsigned char*, char** word, char** psvl, 
    std::vector<size_t>&, 
    std::vector<glm::vec4>& v, 
    std::vector<glm::vec3>&,
    std::vector<glm::vec3>&,
    std::vector<std::vector<glm::vec<3, size_t>>>&
) noexcept
// (unsigned char* vd, char** word, char** psvl, std::vector<glm::vec4>* v)
{
    glm::vec4 vi(0, 0, 0, 1);
    for (unsigned char i = 0; i < 4; *vd = ++i) {
        if ((*word = strtok_r(nullptr, " ", psvl)) == nullptr) break;
        vi[i] = strtof(*word, nullptr);
    }
    vi.z = -vi.z;
    v.push_back(vi);
}

inline constexpr void fun_on_vn(
    unsigned char*, unsigned char*, unsigned char* nd, char** word, char** psvl, 
    std::vector<size_t>&, 
    std::vector<glm::vec4>&, 
    std::vector<glm::vec3>&,
    std::vector<glm::vec3>& n,
    std::vector<std::vector<glm::vec<3, size_t>>>&
) noexcept
// (unsigned char* nd, char** word, char** psvl, std::vector<glm::vec3>* n)
{
    glm::vec3 ni(0);
    for (unsigned char i = 0; i < 3; *nd = ++i) {
        if ((*word = strtok_r(nullptr, " ", psvl)) == nullptr) break;
        ni[i] = strtof(*word, nullptr);
    }
    ni.z = -ni.z;
    n.push_back(ni);
}


inline constexpr void fun_on_vt(
    unsigned char*, unsigned char* td, unsigned char*, char** word, char** psvl, 
    std::vector<size_t>&, 
    std::vector<glm::vec4>&, 
    std::vector<glm::vec3>& t,
    std::vector<glm::vec3>&,
    std::vector<std::vector<glm::vec<3, size_t>>>&
) noexcept
// (unsigned char* td, char** word, char** psvl, std::vector<glm::vec3>* t)
{
    glm::vec3 ti(0);
    for (unsigned char i = 0; i < 3; *td = ++i) {
        if ((*word = strtok_r(nullptr, " ", psvl)) == nullptr) break;
        ti[i] = strtof(*word, nullptr);
    }
    t.push_back(ti);
}


inline void fun_on_f(
    unsigned char*, unsigned char*, unsigned char*, char** word, char** psvl, 
    std::vector<size_t>&, 
    std::vector<glm::vec4>&, 
    std::vector<glm::vec3>&,
    std::vector<glm::vec3>&,
    std::vector<std::vector<glm::vec<3, size_t>>>& f
) noexcept
// (unsigned char* td, char** word, char** psvl, std::vector<std::vector<glm::vec<3, size_t>>>* f)
{
    std::vector<glm::vec<3, size_t>> fi;
    while ((*word = strtok_r(nullptr, " ", psvl)) != nullptr)
    {
        char* psvsh;
        glm::vec<3, size_t> fvi(0);
        *word = strtok_r(*word, "/", &psvsh);

        if (*psvsh == '/')
        {
            fvi.x = static_cast<size_t>(strtoull(*word, nullptr, 10));
            fvi.z = static_cast<size_t>(strtoull(psvsh+1, nullptr, 10));
            fi.push_back(fvi);
            continue;
        }
        else if (*psvsh == '\0'){
            fvi.x = static_cast<size_t>(strtoull(*word, nullptr, 10));
            fi.push_back(fvi);
            continue;
        }
        
        for (unsigned char i = 0; i < 3; ++i) {
            fvi[i] = static_cast<size_t>(strtoull(*word, nullptr, 10));
            if ((*word = strtok_r(nullptr, "/", &psvsh)) == nullptr) break;
        }
        fi.push_back(fvi);
    }
    f.push_back(fi);
}

inline void fun_on_l(
    unsigned char*, unsigned char*, unsigned char*, char** word, char** psvl, 
    std::vector<size_t>& l, 
    std::vector<glm::vec4>&, 
    std::vector<glm::vec3>&,
    std::vector<glm::vec3>&,
    std::vector<std::vector<glm::vec<3, size_t>>>&
) noexcept
// (unsigned char* td, char** word, char** psvl, std::vector<size_t>* l)
{
    while ((*word = strtok_r(nullptr, " ", psvl)) != nullptr)
    {
        l.push_back(static_cast<size_t>(strtoull(*word, nullptr, 10)));
    }
}


namespace px
{
    
    Mesh::Mesh() : model_mat(1), vd(0), td(0), nd(0)
    {}

    Mesh::Mesh(const char* filename)
    : model_mat(1), vd(0), td(0), nd(0)
    {
        // loadModel(filename);
    }

    Mesh::Mesh(size_t size, void* data) : model_mat(1), vd(0), td(0), nd(0)
    {

    }


    void px::Mesh::loadModel(const char * filename, double* t1, double* t2)
    {
        const char* ext = strrchr(filename, '.');
        if (ext == nullptr) perror("error: Invalid file.");
        

        FILE* pFile = fopen(filename, "r");
        if (pFile == nullptr) perror("error: Unable to open file");
        Assimp::Importer importer;
        
        // if (!scene) {std::cout << "Error: " << importer.GetErrorString() << std::endl; std::cout.rdbuf(coutBuffer); /*return 1;*/}
        // else std::cout << "Total meshes = " << scene->mNumMeshes << std::endl;
        fclose(pFile);
        //if (!strcasecmp(ext, ".obj"))
        //{

        //    if (pFile == nullptr) perror("error: Unable to open file");

        //    auto start = std::chrono::high_resolution_clock::now();
        //    this->__from_wavefront(pFile);
        //    auto end = std::chrono::high_resolution_clock::now();
        //    *t1 = static_cast<std::chrono::duration<double>>(end - start).count();

        //    fclose(pFile);
        //    FILE* pFile2 = fopen(filename, "r");
        //    if (pFile2 == nullptr) perror("error: Unable to open file");

        //    vd = td = nd = 0;

        //    start = std::chrono::high_resolution_clock::now();
        //    this->__from_wavefront_v2(pFile2);
        //    end = std::chrono::high_resolution_clock::now();
        //    *t2 = static_cast<std::chrono::duration<double>>(end - start).count();
        //    fclose(pFile2);
        //}
        fclose(pFile);
    }

    void Mesh::scale(double vx, double vy, double vz)
    {
        model_mat = glm::scale(model_mat, glm::vec3(vx, vy, vz));
    }

    void Mesh::rotate(double angle, double ax, double ay, double az)
    {
        model_mat = glm::rotate(model_mat, (float)angle, glm::vec3(ax, ay, az));
    }

    void px::Mesh::translate(double px, double py, double pz)
    {
        model_mat = glm::translate(model_mat, glm::vec3(px, py, pz));
    }

    void Mesh::__from_wavefront(FILE* pOBJ)
    {
        size_t len;
        char* line = nullptr;

        std::vector<size_t> l;
        std::vector<glm::vec4> v;
        std::vector<glm::vec3> n, t;
        std::vector<std::vector<glm::vec<3, size_t>>> f;
        while (getline(&line, &len, pOBJ) != -1)
        {
            line[len-1] = '\0';
            char* psvl, *word = strtok_r((char*)line, " ", &psvl); 

            if (word == nullptr || *word == '#') continue;
            else if (!strcmp(word, "v"))
            {
                glm::vec4 vi(0, 0, 0, 1);
                for (unsigned char i = 0; i < 4; vd = ++i) {
                    if ((word = strtok_r(nullptr, " ", &psvl)) == nullptr) break;
                    vi[i] = strtof(word, nullptr);
                }
                vi.z = -vi.z;
                v.push_back(vi);
            }
            else if (!strcmp(word, "vn"))
            {

                glm::vec3 ni(0);
                for (unsigned char i = 0; i < 3; nd = ++i) {
                    if ((word = strtok_r(nullptr, " ", &psvl)) == nullptr) break;
                    ni[i] = strtof(word, nullptr);
                }
                ni.z = -ni.z;
                n.push_back(ni);
            }
            else if (!strcmp(word, "vt"))
            {

                glm::vec3 ti(0);
                for (unsigned char i = 0; i < 3; td = ++i) {
                    if ((word = strtok_r(nullptr, " ", &psvl)) == nullptr) break;
                    ti[i] = strtof(word, nullptr);
                }
                t.push_back(ti);
            }
            else if (!strcmp(word, "f"))
            {

                std::vector<glm::vec<3, size_t>> fi;
                while ((word = strtok_r(nullptr, " ", &psvl)) != nullptr)
                {
                    char* psvsh;
                    glm::vec<3, size_t> fvi(0);
                    word = strtok_r(word, "/", &psvsh);

                    if (*psvsh == '/')
                    {
                        fvi.x = static_cast<size_t>(strtoull(word, nullptr, 10));
                        fvi.z = static_cast<size_t>(strtoull(psvsh+1, nullptr, 10));
                        fi.push_back(fvi);
                        continue;
                    }
                    else if (*psvsh == '\0'){
                        fvi.x = static_cast<size_t>(strtoull(word, nullptr, 10));
                        fi.push_back(fvi);
                        continue;
                    }
                    
                    for (unsigned char i = 0; i < 3; ++i) {
                        fvi[i] = static_cast<size_t>(strtoull(word, nullptr, 10));
                        if ((word = strtok_r(nullptr, "/", &psvsh)) == nullptr) break;
                    }
                    fi.push_back(fvi);
                }
                f.push_back(fi);
            }
            else if (!strcmp(word, "l"))
            {

                while ((word = strtok_r(nullptr, " ", &psvl)) != nullptr)
                {
                    l.push_back(static_cast<size_t>(strtoull(word, nullptr, 10)));
                }
            }
        }

        // printf("Size: v = %ld, vt = %ld, vn = %ld, f = %ld, l = %ld\n", v.size(), t.size(), n.size(), f.size(), l.size());
        // printf("Dimention: vd = %i, td = %i, nd = %i\n", vd, td, nd);

        // for (auto p : v) printf("v: (%f, %f, %f)\n", p.x, p.y, p.z);
        // for (auto p : t) printf("vt: (%f, %f, %f)\n", p.x, p.y, p.z);
        // for (auto p : n) printf("vn: (%f, %f, %f)\n", p.x, p.y, p.z);
        // for (auto fi : f)
        // {
        //     for (auto p : fi) {
        //         printf("f: (%lu, %lu, %lu)\t", p.x, p.y, p.z);
        //     }
        //     printf("\n");
        // }
        // printf("l: ");
        // for (auto p : l) printf("%zu ", p);
        // printf("\n");

        // dmodel_data.clear(), fmodel_data.clear();

        // if (l.empty())
        // {
            
        // }

        // dmodel_data.insert(dmodel_data.end(), {3.14, 0.1, glm::epsilon<double>()});

    
    }


    void Mesh::__from_wavefront_v2(FILE* pOBJ) noexcept
    {
        size_t len;
        char* line = nullptr;

        std::vector<size_t> l;
        std::vector<glm::vec4> v;
        std::vector<glm::vec3> n, t;
        std::vector<std::vector<glm::vec<3, size_t>>> f;

        std::unordered_map<std::string_view, void (*)(
            unsigned char*, unsigned char*, unsigned char*, char**, char**, 
            std::vector<size_t>&, 
            std::vector<glm::vec4>&, 
            std::vector<glm::vec3>&,
            std::vector<glm::vec3>&,
            std::vector<std::vector<glm::vec<3, size_t>>>&
        )> map;

        map["v"] = fun_on_v;
        map["vt"] = fun_on_vt;
        map["vn"] = fun_on_vn;
        map["f"] = fun_on_f;
        map["l"] = fun_on_l;

        while (getline(&line, &len, pOBJ) != -1)
        {
            line[len-1] = '\0';
            char* psvl, *word = strtok_r((char*)line, " ", &psvl); 

            if (word == nullptr || *word == '#') continue;

            // printf("%s\n", word);
            
            auto pFunc = map.find(word);
            if (pFunc != map.end()) pFunc->second(&vd, &td, &nd, &word, &psvl, l, v, t, n, f);
            // map.at(word)(&vd, &td, &nd, &word, &psvl, l, v, t, n, f);

            // else if (!strcmp(word, "v"))
            // {
            //     fun_on_v(&this->vd, &word, &psvl, &v);
            // }
            // else if (!strcmp(word, "vn"))
            // {
            //     fun_on_vn(&this->vd, &word, &psvl, &n);
            // }
            // else if (!strcmp(word, "vt"))
            // {
            //     fun_on_vt(&this->td, &word, &psvl, &t);
            // }
            // else if (!strcmp(word, "f"))
            // {
            //     fun_on_f(nullptr, &word, &psvl, &f);                
            // }
            // else if (!strcmp(word, "l"))
            // {
            //     fun_on_l(nullptr, &word, &psvl, &l);               
            // }
        }

        // printf("Size: v = %ld, vt = %ld, vn = %ld, f = %ld, l = %ld\n", v.size(), t.size(), n.size(), f.size(), l.size());
        // printf("Dimention: vd = %i, td = %i, nd = %i\n", vd, td, nd);

        // for (auto p : v) printf("v: (%f, %f, %f)\n", p.x, p.y, p.z);
        // for (auto p : t) printf("vt: (%f, %f, %f)\n", p.x, p.y, p.z);
        // for (auto p : n) printf("vn: (%f, %f, %f)\n", p.x, p.y, p.z);
        // for (auto fi : f)
        // {
        //     for (auto p : fi) {
        //         printf("f: (%lu, %lu, %lu)\t", p.x, p.y, p.z);
        //     }
        //     printf("\n");
        // }
        // printf("l: ");
        // for (auto p : l) printf("%zu ", p);
        // printf("\n");

        // dmodel_data.clear(), fmodel_data.clear();

        // if (l.empty())
        // {
            
        // }

        // dmodel_data.insert(dmodel_data.end(), {3.14, 0.1, glm::epsilon<double>()});

    
    }




} // namespace px

#include <stdio.h>
#include <string.h>
#include <glm/matrix.hpp>

#include "PXE/pe_mesh.hpp"

namespace px
{
    
    Mesh::Mesh() : model_mat(1), vd(0), td(0), nd(0)
    {

    }

    Mesh::Mesh(const char* filename) : model_mat(1), vd(0), td(0), nd(0)
    {
        loadModel(filename);
    }

    Mesh::Mesh(size_t size, void* data) : model_mat(1), vd(0), td(0), nd(0)
    {

    }

    Mesh::~Mesh()
    {

    }

    void px::Mesh::loadModel(const char * filename)
    {
        const char* ext = strrchr(filename, '.');
        if (ext == nullptr) perror("error: Invalid file.");
        FILE* pFile = fopen(filename, "r");
        if (pFile == nullptr) perror("error: Unable to open file");
        if (!strcasecmp(ext, ".obj"))
        {
            this->__from_wavefront(pFile);
        }

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
            char* psvl, *word = strtok_r((char*)line, " ", &psvl); 

            if (word == nullptr || *word == '#') break;
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

            }
            else if (!strcmp(word, "l"))
            {

            }
        }

        if (l.empty())
        {

        }

    }


} // namespace px

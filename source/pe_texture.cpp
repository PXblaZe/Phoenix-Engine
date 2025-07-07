#include <string>
#include <stdexcept>

#include "PXE/pe_init.hpp"
#include "PXE/pe_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace px {
    Texture::Texture(): width(0), height(0), channels(0), textureID(0) {}
    
    Texture::Texture(const unsigned char* pixel_data, int width, int height, int channels): width(width), height(height), channels(channels)
    {
        PXcall(glGenTextures(1, &this->textureID));
        PXcall(glBindTexture(GL_TEXTURE_2D, this->textureID));

        PXcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        PXcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        PXcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        PXcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GLenum fmt = (channels == 4) ? GL_RGBA : GL_RGB;
        PXcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, fmt, GL_UNSIGNED_BYTE, pixel_data));
        PXcall(glGenerateMipmap(GL_TEXTURE_2D));

        PXcall(glBindTexture(GL_TEXTURE_2D, 0));
    }
    

    Texture::Texture(const char* filename)
    {
        
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* image =  stbi_load(filename, &width, &height, nullptr, STBI_rgb_alpha);
        channels = 4;
        if (image == nullptr) {
            throw std::runtime_error(std::string("STB_IMAGE unable to load from file - ") + filename);
        }

        PXcall(glGenTextures(1, &this->textureID));
        PXcall(glBindTexture(GL_TEXTURE_2D, this->textureID));

        PXcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        PXcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        PXcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        PXcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        PXcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLubyte*>(image)));
        PXcall(glGenerateMipmap(GL_TEXTURE_2D));

        PXcall(glBindTexture(GL_TEXTURE_2D, 0));

        stbi_image_free(image);
    }

    Texture::~Texture()
    {
        clearTex();
    }

    void Texture::useTex(unsigned int textureUnit) const
    {
        PXcall(glActiveTexture(GL_TEXTURE0 + textureUnit));
        PXcall(glBindTexture(GL_TEXTURE_2D, this->textureID));
        // glGetUniformLocation(this->programid, name)
        // glUniform1i()
    }

    void Texture::idle()
    {
        PXcall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void Texture::clearTex()
    {
        PXcall(glDeleteTextures(1, &this->textureID));
        this->textureID = 0;
        this->width = 0;
        this->height = 0;
        this->channels = 0;
    }

    int Texture::getWidth() const {return this->width;}
    int Texture::getHeight() const {return this->height;}
    unsigned int Texture::getChannels() const {return this->channels;}
    unsigned int Texture::getTexID() const {return this->textureID;}

} // namespace px
// #undef STB_IMAGE_IMPLEMENTATION

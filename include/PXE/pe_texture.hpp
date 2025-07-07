#pragma once

namespace px 
{
    class Texture
    {
        unsigned int textureID;
        int width, height, channels;
    public:
        Texture();
        ~Texture();
        Texture(const unsigned char* pixel_data, int width, int height, int channels);
        Texture(const char* filename);

        void clearTex();
        int getWidth() const;
        int getHeight() const;
        unsigned int getTexID() const;
        unsigned int getChannels() const;
        void useTex(unsigned int textureUnit = 0) const;

        static void idle();
    }; 
} // namespace px  


#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GL/glew.h> 
#include <iostream>
using namespace std;
unsigned int TextureManager::Load(const char* path)
{
    if (textures.find(path) == textures.end())
    {
        textures[path] = { TextureFromFile(path, "../textures"), textureUnit++ };
    }
    return  textures[path].first;
}

void TextureManager::SetTexture(unsigned int uniformLocation, const char* path)
{
    unsigned tex = Load(path);
    glUniform1i(uniformLocation, textures[path].second);
    glActiveTexture(GL_TEXTURE0 + textures[path].second);
    glBindTexture(GL_TEXTURE_2D, tex);
}


unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        while (GLenum error = glGetError())
        {
            std::cout << "OpenGL Error: " << error << std::endl;
        }
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
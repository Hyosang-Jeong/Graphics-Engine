#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <GL/glew.h> 
#include <iostream>
#include <glm/ext/scalar_constants.hpp>
#include "stb_image_write.h"
using namespace std;
Texture TextureManager::Load(const char* path)
{
    if (textures.find(path) == textures.end())
    {
        textures[path] =  TextureFromFile(path, "../textures");
         textures[path].unit = unit++;
    }
    return  textures[path];
}

void TextureManager::SetTexture(unsigned int uniformLocation, const char* path)
{
    unsigned tex = Load(path).id;
    glUniform1i(uniformLocation, textures[path].unit);
    glActiveTexture(GL_TEXTURE0 + textures[path].unit);
    glBindTexture(GL_TEXTURE_2D, tex);
}


Texture TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;
    stbi_set_flip_vertically_on_load(true);
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

    return Texture(path,textureID, width, height);
}

Texture::Texture(std::string path,unsigned int id, unsigned int width, unsigned int height) :path(path),id(id), width(width), height(height)
{
}

std::vector<glm::vec3> getImageDataFromTexture(Texture texture) {
	std::vector<glm::vec3> imageData(texture.width * texture.height);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, imageData.data());
	return imageData;
}



std::vector<glm::vec3> calculate_Irr_coefficients(Texture hdr)
{
    
    std::vector<glm::vec3> hdrdata = getImageDataFromHDR("../textures/"+hdr.path);

	//std::vector<glm::vec3> hdr_data = getImageDataFromTexture(hdr);
	const float PI = glm::pi<float>();
	const float PI2 = 2.f * PI;


	//Clamped dot product to spherical harmonic coefficients
	std::vector<float> A;
	A.resize(3);
	A[0] = PI;
	A[1] = (2.f / 3.f) * PI;
	A[2] = (1.f / 4.f) * PI;

	//The spherical harmonic basis functions
	std::vector<float> Y;
	Y.resize(9);

	Y[0] = (0.5f) * sqrt(1.f / PI);

	Y[1] = (0.5f) * sqrt(3.f / PI);
	Y[2] = (0.5f) * sqrt(3.f / PI);
	Y[3] = (0.5f) * sqrt(3.f / PI);

	Y[4] = (0.5f) * sqrt(15.f / PI);
	Y[5] = (0.5f) * sqrt(15.f / PI);
	Y[6] = (0.25f) * sqrt(5.f / PI);
	Y[7] = (0.5f) * sqrt(15.f / PI);
	Y[8] = (0.25f) * sqrt(15.f / PI);


	//Input image to spherical harmonic coefficients:
	std::vector<glm::vec3> L{ 9,glm::vec3(0) };
	float dTheta = PI / (float)hdr.height;
	float dpi = PI2 / (float)hdr.width;

	for (int j = 0; j < hdr.height; j++)
	{
		for (int i = 0; i < hdr.width; i++)
		{
			float theta = PI * (j + 0.5) / (float)hdr.height;
			float phi = PI2 * (i + 0.5) / (float)hdr.width;

			glm::vec3 color = hdrdata[j * hdr.width + i];
			glm::vec3 xyz = glm::vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));

			L[0] += color * Y[0] * sin(theta) * dTheta * dpi;
			L[1] += color * Y[1] * xyz.y * sin(theta) * dTheta * dpi;
			L[2] += color * Y[2] * xyz.z * sin(theta) * dTheta * dpi;
			L[3] += color * Y[3] * xyz.x * sin(theta) * dTheta * dpi;
			L[4] += color * Y[4] * xyz.x * xyz.y * sin(theta) * dTheta * dpi;
			L[5] += color * Y[5] * xyz.y * xyz.z * sin(theta) * dTheta * dpi;
			L[6] += color * Y[6] * ((xyz.z * xyz.z * 3) - 1) * sin(theta) * dTheta * dpi;
			L[7] += color * Y[7] * xyz.x * xyz.z * sin(theta) * dTheta * dpi;
			L[8] += color * Y[8] * ((xyz.x * xyz.x) - (xyz.y * xyz.y)) * sin(theta) * dTheta * dpi;
		}
	}

	std::vector<glm::vec3> E{ 9};

	E[0] = A[0] * L[0];
	E[1] = A[1] * L[1];
	E[2] = A[1] * L[2];
	E[3] = A[1] * L[3];
	E[4] = A[2] * L[4];
	E[5] = A[2] * L[5];
	E[6] = A[2] * L[6];
	E[7] = A[2] * L[7];
	E[8] = A[2] * L[8];

	return E;
}

void writeIrr_image(std::string file_name, std::vector<glm::vec3> E)
{
    const float PI = glm::pi<float>();
    const float PI2 = 2.f * PI;
    //The spherical harmonic basis functions
    std::vector<float> Y;
    Y.resize(9);

    Y[0] = (0.5f) * sqrt(1.f / PI);

    Y[1] = (0.5f) * sqrt(3.f / PI);
    Y[2] = (0.5f) * sqrt(3.f / PI);
    Y[3] = (0.5f) * sqrt(3.f / PI);

    Y[4] = (0.5f) * sqrt(15.f / PI);
    Y[5] = (0.5f) * sqrt(15.f / PI);
    Y[6] = (0.25f) * sqrt(5.f / PI);
    Y[7] = (0.5f) * sqrt(15.f / PI);
    Y[8] = (0.25f) * sqrt(15.f / PI);

    std::vector<float> textureData(400 * 200 * 3);
    for (int j = 0; j < 200; ++j) {
        for (int i = 0; i < 400; ++i) {

            float theta = glm::pi<float>() * j / 200;
            float phi = 2.0f * glm::pi<float>() * i / 400;
            glm::vec3 xyz = glm::vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));

            glm::vec3 irradiance = glm::vec3(0.0);
            irradiance += Y[0] * E[0];
            irradiance += Y[1] * E[1] * xyz.y;
            irradiance += Y[2] * E[2] * xyz.z;
            irradiance += Y[3] * E[3] * xyz.x;
            irradiance += Y[4] * E[4] * xyz.x * xyz.y;
            irradiance += Y[5] * E[5] * xyz.y * xyz.z;
            irradiance += Y[6] * E[6] * ((xyz.z * xyz.z * 3) - 1);
            irradiance += Y[7] * E[7] * xyz.x * xyz.z;
            irradiance += Y[8] * E[8] * ((xyz.x * xyz.x) - (xyz.y * xyz.y));


            int index = (j * 400 + i) * 3;
            textureData[index] = glm::clamp(irradiance.r, 0.0f, 1.0f);
            textureData[index + 1] = glm::clamp(irradiance.g, 0.0f, 1.0f);
            textureData[index + 2] = glm::clamp(irradiance.b, 0.0f, 1.0f);
        }
    }

    stbi_write_hdr(file_name.c_str(), 400, 200, 3, textureData.data());

}

std::vector<glm::vec3> getImageDataFromHDR(std::string path)
{
    stbi_set_flip_vertically_on_load(false);
    int width, height;
    float* data = stbi_loadf(path.c_str(), &width, &height, nullptr, 3);
    if (!data) {
        throw std::runtime_error("Failed to load HDR image: ");
    }

    std::vector<glm::vec3> hdrdata;

    hdrdata.reserve(width * height);

    for (int i = 0; i < width * height; ++i) {
        hdrdata.push_back(glm::vec3(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]));
    }

    stbi_image_free(data);
    return hdrdata;
}

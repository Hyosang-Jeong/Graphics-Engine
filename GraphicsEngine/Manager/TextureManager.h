#pragma once
#include<map>
#include<string>
#include<glm/glm.hpp>
#include<vector>
#include<glm/common.hpp>
class Texture
{
public:
	Texture() = default;
	Texture(std::string path, unsigned int id, unsigned int width, unsigned int height);
	unsigned int id;
	unsigned int width;
	unsigned int height;
	unsigned int unit;
	std::string path;
};


class TextureManager
{
public:
	Texture Load(const char* path);
	void SetTexture(unsigned int uniformLocation, const char* path);
private:
	std::map<std::string, Texture> textures;
	unsigned int unit{ 0 };
};

Texture TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
std::vector<glm::vec3> getImageDataFromTexture(Texture texture);
std::vector<glm::vec3> calculate_Irr_coefficients(Texture hdr);
void writeIrr_image(std::string file_name, std::vector<glm::vec3> coefficients);
std::vector<glm::vec3> getImageDataFromHDR(std::string path);
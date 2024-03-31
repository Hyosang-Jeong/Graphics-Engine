#pragma once
#include<map>
#include<string>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class TextureManager
{
public:
	unsigned int Load(const char* path);
	void SetTexture(unsigned int uniformLocation, const char* path);
private:
	std::map<std::string, std::pair<unsigned int, unsigned int>> textures;
	unsigned int textureUnit{ 0 };
};
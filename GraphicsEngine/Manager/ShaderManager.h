#pragma once
#include<map>
#include<string>
class Shader;

class ShaderManager
{
public:
	Shader* Load(const char* vertexPath, const char* fragmentPath);
	Shader* Load(const char* computePath);
	void UnLoad();
private:
	std::map<std::pair<std::string, std::string>, Shader*> shaders;
};
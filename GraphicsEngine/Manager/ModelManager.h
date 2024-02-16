#pragma once
#include<unordered_map>
#include<string>
class Model;

class ModelManager
{
public:
	Model* Load(const char*Path);
	void UnLoad();
private:
	std::unordered_map<std::string, Model*> models;
};
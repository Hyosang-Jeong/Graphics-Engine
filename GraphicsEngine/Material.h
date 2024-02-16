#pragma once
#include<glm/glm.hpp>
#include"shader.h"
#include"Engine.h"
class Material
{
public:
	Material() = default;
	virtual void SetMaterial(unsigned texture_id) = 0;
	virtual void SetMaterial(glm::vec3 material_value) = 0;
	virtual void SendMaterial(Shader* shader) = 0;
};

class Diffuse : public Material
{
public:
	Diffuse() {
		diffuse_texture = -1;
		diffuse_value = glm::vec3(1, 1, 1);
		useTexture = false;
	}
	virtual void SetMaterial(std::string texture_path) { diffuse_texture = texture_path; }
	virtual void SetMaterial(glm::vec3 material_value) { diffuse_value = material_value; }
	virtual void SendMaterial(Shader* shader) 
	{
		if (useTexture)
		{
			shader->setBool("useDiffuseTexture", true);

			unsigned int location = glGetUniformLocation(shader->ID, "diffuseTexture");

			Engine::GetTextureManager().SetTexture(location,diffuse_texture.c_str());
		}
		else
		{
			shader->setBool("useDiffuseTexture", false);
			shader->setVec3("diffuseValue", diffuse_value);
		}
	}
private:
	bool useTexture;
	std::string diffuse_texture;
	glm::vec3 diffuse_value;
};

class Specular : public Material
{
public:
	Specular() {
		specular_texture = -1;
		specular_value = glm::vec3(1, 1, 1);
	}
	virtual void SetMaterial(unsigned texture_id) { specular_texture = texture_id; }
	virtual void SetMaterial(glm::vec3 material_value) { specular_value = material_value; }
	virtual void SendMaterial(Shader* shader) = 0;
private:
	unsigned  specular_texture;
	glm::vec3 specular_value;
};
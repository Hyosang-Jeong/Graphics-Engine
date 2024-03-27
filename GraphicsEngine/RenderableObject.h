#pragma once
#include"Model.h"
#include"Material.h"

class RenderableObject
{
public:
	RenderableObject() = default;
	RenderableObject(Model* model, Transform transform = Transform());
	virtual void Rendering(Shader* shader);
    void SetPosItion(glm::vec3 pos);
    void SetRotation(glm::vec3 angle);
    void SetScale(glm::vec3 scale);
    void SetDiffuse(std::string texture_path);
    void SetDiffuse(glm::vec3 value);
    void SetSpecular(std::string texture_path);
    void SetSpecular(glm::vec3 value);
    void SetRoughness(float rough);
    void SetMetalness(float metal);
    glm::vec3 GetPostion();
    glm::vec3 GetRotation();
    glm::vec3 GetScale();
    glm::vec3 GetDiffuseColor();
    glm::vec3 GetSpecularColor();
   float GetRoughness();
   float GetMetalness();
protected:
	Transform transform;
    bool useDiffuseTexture{ false };
    std::string diffuse_texture;
    glm::vec3 diffuse_value{ 0.5 };

    bool useSpecularTexture{ false };
    std::string specular_texture;
    glm::vec3 specular_value{ 0.5 };

    float roughness{ 100 };
    float metalness{ 0.4 };

private:
	Model* model{ nullptr };
};

class Quad: public RenderableObject
{
public:
    Quad();
    virtual void Rendering(Shader* shader) override;
    glm::vec3 pos{ 0 };
    glm::vec3 scale{ 1 };
    glm::vec3 rotate{ 0 };
private:
    unsigned int quadVAO, quadVBO;
};


class Sphere : public RenderableObject
{
public:
    Sphere();
    Sphere(glm::vec3 center, float radius);
    virtual void Rendering(Shader* shader) override;
     float GetRadius();
    glm::vec3 center;
private:
    unsigned int VAO, VBO,EBO,count;
};
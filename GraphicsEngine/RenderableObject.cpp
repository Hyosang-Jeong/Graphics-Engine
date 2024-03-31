#include "RenderableObject.h"
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
RenderableObject::RenderableObject(Model* model, Transform transform):model(model),transform(transform)
{}

void RenderableObject::Rendering(Shader* shader)
{
    shader->active();
    shader->setMat4("model", transform.GetModelMatrix());
    if (useDiffuseTexture)
    {
        shader->setBool("useDiffuseTexture", true);
        unsigned int location = glGetUniformLocation(shader->ID, "diffuseTexture");
        Engine::GetTextureManager().SetTexture(location, diffuse_texture.c_str());
    }
    else
    {
        shader->setBool("useDiffuseTexture", false);
        shader->setVec3("diffuseValue", diffuse_value);
    }
    if (useSpecularTexture)
    {
        shader->setBool("useSpecularTexture", true);
        unsigned int location = glGetUniformLocation(shader->ID, "SpecularTexture");
        Engine::GetTextureManager().SetTexture(location, specular_texture.c_str());
    }
    else
    {
        shader->setBool("useSpecularTexture", false);
        shader->setVec3("SpecularValue", specular_value);
    }
    model->Rendering();
}

void RenderableObject::SetPosItion(glm::vec3 pos)
{
    transform.position = pos;
}

void RenderableObject::SetRotation(glm::vec3 angle)
{
    transform.rotate = angle;
}

void RenderableObject::SetScale(glm::vec3 scale)
{
    transform.scale = scale;
}

void RenderableObject::SetDiffuse(std::string texture_path)
{
    useDiffuseTexture = true;
    diffuse_texture = texture_path;
}

void RenderableObject::SetDiffuse(glm::vec3 value)
{
    useDiffuseTexture = false;
    diffuse_value = value;
}

void RenderableObject::SetSpecular(std::string texture_path)
{
    useSpecularTexture = true;
    specular_texture = texture_path;
}

void RenderableObject::SetSpecular(glm::vec3 value)
{
    useSpecularTexture = false;
    specular_value = value;
}

glm::vec3 RenderableObject::GetPostion()
{
    return transform.position;
}

glm::vec3 RenderableObject::GetRotation()
{
    return transform.rotate;
}

glm::vec3 RenderableObject::GetScale()
{
    return transform.scale;
}

glm::vec3 RenderableObject::GetDiffuseColor()
{
    return diffuse_value;
}
glm::vec3 RenderableObject::GetSpecularColor()
{
    return specular_value;
}
Quad::Quad()
{
    float quadVertices[] = {
        // positions       //normals      // texture Coords
        -1.0f,  1.0f, 0.0f,  0.0f,0.0f,1.f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,0.0f,1.f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   0.0f,0.0f,1.f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  0.0f,0.0f,1.f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


}

void Quad::Rendering(Shader* shader)
{
    shader->active();
    shader->setMat4("model", transform.GetModelMatrix());
    if (useDiffuseTexture)
    {
        shader->setBool("useDiffuseTexture", true);
        unsigned int location = glGetUniformLocation(shader->ID, "diffuseTexture");
        Engine::GetTextureManager().SetTexture(location, diffuse_texture.c_str());
    }
    else
    {
        shader->setBool("useDiffuseTexture", false);
        shader->setVec3("diffuseValue", diffuse_value);
    }
    if (useSpecularTexture)
    {
        shader->setBool("useSpecularTexture", true);
        unsigned int location = glGetUniformLocation(shader->ID, "SpecularTexture");
        Engine::GetTextureManager().SetTexture(location, specular_texture.c_str());
    }
    else
    {
        shader->setBool("useSpecularTexture", false);
        shader->setVec3("SpecularValue", specular_value);
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

Sphere::Sphere():RenderableObject()
{
    model = Engine::GetModelManager().Load("../obj/sphere.obj");
    center = glm::vec3(0, 0, 0);
    transform.scale = glm::vec3(1);
}

Sphere::Sphere(glm::vec3 center_, float radius):Sphere()
{
    center = center_;
    transform.scale = glm::vec3(radius);
}

void Sphere::Rendering(Shader* shader)
{
    shader->active();
    shader->setMat4("model", transform.GetModelMatrix());
    if (useDiffuseTexture)
    {
        shader->setBool("useDiffuseTexture", true);
        unsigned int location = glGetUniformLocation(shader->ID, "diffuseTexture");
        Engine::GetTextureManager().SetTexture(location, diffuse_texture.c_str());
    }
    else
    {
        shader->setBool("useDiffuseTexture", false);
        shader->setVec3("diffuseValue", diffuse_value);
    }
    if (useSpecularTexture)
    {
        shader->setBool("useSpecularTexture", true);
        unsigned int location = glGetUniformLocation(shader->ID, "SpecularTexture");
        Engine::GetTextureManager().SetTexture(location, specular_texture.c_str());
    }
    else
    {
        shader->setBool("useSpecularTexture", false);
        shader->setVec3("SpecularValue", specular_value);
    }
    model->Rendering();
}

float Sphere::GetRadius()
{
    return transform.scale.x;
}

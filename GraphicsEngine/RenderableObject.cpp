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

    shader->setFloat("roughness", roughness);
    shader->setFloat("metalness", metalness);


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

void RenderableObject::SetRoughness(float rough)
{
    roughness = rough;
}

void RenderableObject::SetMetalness(float metal)
{
    metalness = metal;
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
float RenderableObject::GetRoughness()
{
    return roughness;
}
float RenderableObject::GetMetalness()
{
    return metalness;
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
    shader->setFloat("roughness", roughness);
    shader->setFloat("metalness", metalness);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
    glBindVertexArray(0);
}

Sphere::Sphere()
{
    int stacks = 100;
    int sectors = 100;
    float radius = 1;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float M_PI = glm::pi<float>();
    float M_PI_2 = M_PI / 2.f;
    float const R = 1.0f / (float)(stacks - 1);
    float const S = 1.0f / (float)(sectors - 1);

    for ( int r = 0; r < stacks; ++r) {
        for ( int s = 0; s < sectors; ++s) {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            vertices.push_back(x); // Normal
            vertices.push_back(y);
            vertices.push_back(z);

            vertices.push_back(s * S); // Texture coordinates
            vertices.push_back(r * R);
        }
    }

    indices.reserve(stacks * sectors * 4);
    for ( int r = 0; r < stacks - 1; ++r) {
        for ( int s = 0; s < sectors - 1; ++s) {
            indices.push_back(r * sectors + s);
            indices.push_back((r + 1) * sectors + s);
            indices.push_back(r * sectors + (s + 1));

            indices.push_back(r * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + s);
            indices.push_back((r + 1) * sectors + (s + 1));
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    count = static_cast<int>(indices.size());
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
    shader->setFloat("roughness", roughness);
    shader->setFloat("metalness", metalness);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

float Sphere::GetRadius()
{
    return transform.scale.x;
}

#include "ShaderManager.h"
#include"../shader.h"
Shader* ShaderManager::Load(const char* vertexPath, const char* fragmentPath)
{
    if (shaders.find({ vertexPath,fragmentPath })  == shaders.end())
    {
        shaders[{ vertexPath, fragmentPath }] =new Shader(vertexPath, fragmentPath);
    }

    return shaders[{ vertexPath, fragmentPath }];
}

Shader* ShaderManager::Load(const char* computePath)
{
    if (shaders.find({ computePath,computePath }) == shaders.end())
    {
        shaders[{ computePath, computePath }] = new Shader(computePath);
    }

    return shaders[{ computePath, computePath }];
}

void ShaderManager::UnLoad()
{
    for (auto& i : shaders)
    {
        delete i.second;
    }
    shaders.clear();
}

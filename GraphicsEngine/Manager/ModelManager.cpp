#include "ModelManager.h"
#include"../Model.h"
Model* ModelManager::Load(const char* Path)
{
        if (models.find(Path) == models.end())
        {
            models[Path] = new Model(Path);
        }

        return models[Path];
}

void ModelManager::UnLoad()
{
    for (auto& i : models)
    {
        delete i.second;
    }
    models.clear();
}

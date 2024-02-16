#include "SceneManager.h"

void SceneManager::AddScene(Scene* scene)
{
    scenes.push_back(scene);
}

void SceneManager::SetNextScene(int index)
{
    scenes[currentScene]->UnLoad();
    currentScene = index;
}

Scene* SceneManager::GetScene()
{
    return scenes[currentScene];
}

void SceneManager::UnLoad()
{
    for (auto& i : scenes)
    {
        delete i;
        i = nullptr;
    }
}

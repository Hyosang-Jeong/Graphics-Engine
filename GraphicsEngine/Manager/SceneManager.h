#pragma once
#include<vector>
#include<Scene/Scene.h>
class SceneManager
{
public:
	void AddScene(Scene* scene);
	void SetNextScene(int index);
	Scene* GetScene();
	void UnLoad();
private:
	std::vector<Scene*> scenes;
	int currentScene{ 0 };
};
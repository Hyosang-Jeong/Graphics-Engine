#pragma once
#include<vector>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include"Scene/Scene.h"
#include"Manager/ShaderManager.h"
#include"Manager/SceneManager.h"
#include"Manager/ModelManager.h"
#include"Manager/TextureManager.h"
#include"Camera.h"
class Engine
{
public:
	static Engine& Instance()
	{
		static Engine instance; 
		return instance;
	}
	static ShaderManager& GetShaderManager() { return Instance().shaderManager; }
	static SceneManager& GetSceneManager() { return Instance().sceneManager; }
	static ModelManager& GetModelManager() { return Instance().modelManager; }
	static TextureManager& GetTextureManager() { return Instance().textureManager; }
	static Camera* GetCamera() { return Instance().camera; }
	static GLFWwindow* GetWindow() { return Instance().ptr_window; };
	static glm::uvec2 GetWindowSize() { return Instance().windowSize; }
	Engine();
	~Engine();
	void init(unsigned w, unsigned h);
	void Update();
	void Draw();
	bool ShouldClose();
	
private:
	GLFWwindow* ptr_window;
	ShaderManager shaderManager;
	SceneManager sceneManager;
	ModelManager modelManager;
	TextureManager textureManager;
	Camera* camera;
	glm::uvec2 windowSize;
};
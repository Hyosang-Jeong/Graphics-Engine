#pragma once
#include"Scene.h"
#include "Rendering.h"
class Camera;
class RenderableObject;
class Shader;
class DeferredShadingScene :public Scene
{
public:
	DeferredShadingScene();
	~DeferredShadingScene();
	void init() override {}
	void Update(float deltaTime)  override;
	void Draw() override;
	void UnLoad() override {}
	void OnImGuiRender() override;
	void DeferredPass();
	void LocalLightsPass();
	void ForwardPass();
	void SetupObjects();
	void SetupLocalLights();
	void SetupShader();
	void ImGUI_Texture();
	void ImGUI_Light();
	void ImGUI_Model();
private:
	Camera* camera;

	RenderableObject* background;
	std::vector<RenderableObject*> objects;
	std::vector<RenderableObject*> lights;

	DeferredRendering deferredRendering;
	ForwardRendering fowardRendering;

	Shader* firstPassShader;
	Shader* secondPassShader;
	Shader* local_lightsShader;
	Shader* backgroundShader;
	Shader* debugShader;


	unsigned int numLight;
	bool drawLightRange[8]{ false };
};
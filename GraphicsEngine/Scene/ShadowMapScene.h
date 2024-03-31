#pragma once
#include"Scene.h"
#include "Rendering.h"
#include"FBO.h"
#include"Equation.h"
class Camera;
class RenderableObject;
class Shader;

class ShadowMapScene :public Scene
{
public:
	ShadowMapScene();
	~ShadowMapScene();
	void init() override {}
	void Update(float deltaTime)  override;
	void Draw() override;
	void UnLoad() override {}
	void OnImGuiRender() override;
	void SetupObjects();
	void SetupShader();

	void ForwardPass();
	void ShadowPass();
	void DeferredPass();

private:
	Camera* camera;
	std::vector<RenderableObject*> objects;
	DeferredRendering deferredRendering;
	ForwardRendering fowardRendering;
	ShadowMapping shadowMapping;
	RenderableObject* background;


	Shader* shadowShader;
	Shader* blurXShader;	
	Shader* blurYShader;	
	Shader* firstPassShader;
	Shader* secondPassShader;
	Shader* backgroundShader;
	Shader* fsqDebugShader;

	Quad fsq;

	glm::vec3 lightPos;
	glm::mat4 lightView;
	glm::vec3 lightColor;

	unsigned image_w = 1280;
	unsigned image_h = 1280;
	int blurwidth = 5;


	float light_near{ 0.1f };
	float light_far{ 23.f }; 

	float exposure = 2.5f;



	int drawingIndex{ 0 };
	bool useBlurTexture{ true };
	bool FromLightView{ false };

	int block_N{ 40 };

	BLOCK block;
	unsigned int id, bindpoint;
};


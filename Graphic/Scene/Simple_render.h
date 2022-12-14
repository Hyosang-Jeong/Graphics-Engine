/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Simple_render.h
Purpose: First demo scene
Language: c++ Microsoft Visual Studio
Platform: Microsoft Visual Studio2019, Windows
Project:  Hyosang Jung_CS300_1
Author: Hyosang Jung, hyosang.jung, 055587
Creation date: 2022 - 09 - 15
End Header --------------------------------------------------------*/
#pragma once
#include"Scene.h"
#include"Model.h"
#include"Object.h"
#include"Camera.h"
#include"Light.h"
#include"LightUBO.h"
#include"FBO.h"
struct ImGui_bool
{
	bool draw_vtx_normal{ false };
	bool draw_face_normal{ false };
	bool rotation{ true };
	bool UseReflaction{ true };
	bool UseRefraction{ true };
};


enum  model_name
{
	Bunny,
	_4Sphere,
	Cube,
	sphere_modified,
	sphere, //from obj file
	Sphere, //from code
	Plane,
	Skybox,
	count
};


class Simple_render : public Scene
{
public:
	Simple_render();
	~Simple_render();
	void init() override {}
	void Update(float deltaTime)  override;
	void Draw() override;
	void UnLoad() override;
	void OnImGuiRender() override;
	
private:
	void create_shaders();
	void create_models();
	void create_spheres_and_lines();
	void create_objects();
	void create_lights();
	void load_skybox();
	void setup_skyboxTransform();
	void reload_shaders(std::string shader);
	void LightImGui();
	void GlobalImGui();
	void EnvironmentImGui();
	void Draw_CenterObj();
	void Draw_Lights();
	void Draw_Skybox();
	void Draw_Skybox_to_Framebuffer();
	void Scenario1();
	void Scenario2();
	void Scenario3();
	Camera camera;

	Model* models[count];
	std::map<std::string, GLint> shaders;

	Object centerObj;
	Object SkyBox;
	Material material;

	std::vector<Object> lightObj;
	LightProperties lights;
	LightUBO lightUBO;

	ImGui_bool flag;

	const int maxLight = 16;
	unsigned int metal_diff_ppm;
	unsigned int metal_spec_ppm;
	std::vector<unsigned int> skyBoxTextures;
	std::vector<unsigned int> emptyTextures;
	FBO fbo;
	glm::mat4 skyBoxTransform[6];
	float refractive_index = 1.0003f;
	float FresnelPower = 0.5f;
	float PhongEnvironmentRatio = 0.9f;
};

#pragma once
#include "DeferredShadingScene.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include"../Engine.h"
#include"Camera.h"
#include"RenderableObject.h"
#include"Rendering.h"
#include <GL/glew.h>
DeferredShadingScene::DeferredShadingScene()
{
	camera = Engine::GetCamera();

	SetupObjects();
	SetupShader();
	SetupLocalLights();
}

DeferredShadingScene::~DeferredShadingScene()
{
	delete background;
	for (auto& light : lights)
	{
		delete light;
	}
	for (auto& object : objects)
	{
		delete object;
	}
}

void DeferredShadingScene::Update(float deltaTime)
{
	camera->Update(deltaTime);
}

void DeferredShadingScene::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//deferred shading
	DeferredPass();
	

	//local lights
	LocalLightsPass();


	//background
	ForwardPass();


	OnImGuiRender();
}

void DeferredShadingScene::OnImGuiRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGUI_Texture();
	ImGUI_Model();
	ImGUI_Light();
}

void DeferredShadingScene::DeferredPass()
{
	firstPassShader->active();
	firstPassShader->setMat4("projection", projection);
	firstPassShader->setMat4("view", camera->GetView());
	secondPassShader->active();
	secondPassShader->setVec3("eyePosition", camera->GetPosition());
	secondPassShader->setVec3("lightPosition", glm::vec3(0, 100, 100));
	deferredRendering.Render(objects, firstPassShader, secondPassShader);
}

void DeferredShadingScene::LocalLightsPass()
{
	local_lightsShader->active();
	local_lightsShader->setMat4("projection", projection);
	local_lightsShader->setMat4("view", camera->GetView());
	local_lightsShader->setVec3("eyePosition", camera->GetPosition());
	deferredRendering.LocalLightsPass(lights, local_lightsShader);
}

void DeferredShadingScene::ForwardPass()
{
	deferredRendering.CopyDepthInfo();
	glCullFace(GL_FRONT);
	backgroundShader->active();
	backgroundShader->setMat4("projection", projection);
	backgroundShader->setMat4("view", glm::mat4(camera->GetView()));
	fowardRendering.Render({ background }, backgroundShader);
	glCullFace(GL_BACK);

	debugShader->active();
	debugShader->setMat4("projection", projection);
	debugShader->setMat4("view", camera->GetView());

	glm::vec3 prevscale[8];
	for (unsigned i = 0; i < 8; i++)
	{
		prevscale[i] = lights[i]->GetScale();
		lights[i]->SetScale(glm::vec3(0.1f));
	}
	fowardRendering.Render( lights, debugShader);

	for (unsigned i = 0; i < 8; i++)
	{
		lights[i]->SetScale(prevscale[i]);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (unsigned i = 0; i < 8; i++)
	{
		if(drawLightRange[i])
			fowardRendering.Render({ lights[i] }, debugShader);
	}
	glDisable(GL_BLEND);


}

void DeferredShadingScene::SetupObjects()
{
	objects.push_back(new RenderableObject(Engine::GetModelManager().Load("../obj/bunny_high_poly.obj")));
	objects.push_back(new RenderableObject(Engine::GetModelManager().Load("../obj/bunny_high_poly.obj")));
	objects.push_back(new RenderableObject(Engine::GetModelManager().Load("../obj/bunny_high_poly.obj")));
	objects.push_back(new Quad());

	objects[0]->SetSpecular(glm::vec3(1, 1, 1));
	objects[1]->SetSpecular(glm::vec3(0.5, 0.5, 0.5));
	objects[2]->SetSpecular(glm::vec3(0, 0, 0));

	objects[0]->SetSpecular(glm::vec3(1, 1, 1));
	objects[1]->SetSpecular(glm::vec3(0.5, 0.5, 0.5));
	objects[2]->SetSpecular(glm::vec3(0, 0, 0));

	objects[0]->SetDiffuse(glm::vec3(0, 0, 0));
	objects[1]->SetDiffuse(glm::vec3(1, 1, 1));
	objects[2]->SetDiffuse(glm::vec3(0.8, 0.3, 0.1));

	objects[0]->SetPosItion(glm::vec3(2.5, 0, 0));
	objects[1]->SetPosItion(glm::vec3(0, 0, 0));
	objects[2]->SetPosItion(glm::vec3(-2.5, 0, 0));

	objects[3]->SetDiffuse("Brazilian_rosewood_pxr128.png");
	objects[3]->SetRotation({ -90,0,0 });
	objects[3]->SetPosItion({ 0,-4,0 });
	objects[3]->SetScale({ 10,10,1 });



	background = new Sphere();
	background->SetDiffuse("Tropical_Beach_8k.jpg");
	background->SetScale({ 100,100,100 });
	background->SetRotation({ 0,180,0 });
}

void DeferredShadingScene::SetupLocalLights()
{
	numLight = 8;
	auto angle = (360.f / numLight) * (glm::pi<float>() / 180.f);
	
	for (unsigned i = 0; i < numLight; i++)
	{
		lights.push_back(new Sphere());
		lights[i]->SetPosItion(glm::vec3(4.f * cos(angle * i), -1.f, 4.f * sin(angle * i)));
		lights[i]->SetScale(glm::vec3(5.5f));
		lights[i]->SetDiffuse(glm::vec3(1.f));
	}
}

void DeferredShadingScene::SetupShader()
{
	firstPassShader = Engine::GetShaderManager().Load("deferred_first.vert", "deferred_first.frag");
	secondPassShader = Engine::GetShaderManager().Load("deferred_second.vert", "deferred_second.frag");
	local_lightsShader = Engine::GetShaderManager().Load("local_lights.vert", "local_lights.frag");
	backgroundShader = Engine::GetShaderManager().Load("background.vert", "background.frag");
	debugShader = Engine::GetShaderManager().Load("debug.vert", "debug.frag");
}

void DeferredShadingScene::ImGUI_Texture()
{
	ImGui::Begin("Debug view");

	float width = 300;
	float height = 300;
	const G_buffer* gbuffer = deferredRendering.GetGbuffer();

	ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)gbuffer->gPosition, ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + width, ImGui::GetCursorScreenPos().y + height), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)gbuffer->gNormal, ImVec2(ImGui::GetCursorScreenPos().x + width, ImGui::GetCursorScreenPos().y),
		ImVec2(ImGui::GetCursorScreenPos().x + width * 2.f, ImGui::GetCursorScreenPos().y + height), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)gbuffer->gDiffuse, ImVec2(ImGui::GetCursorScreenPos().x + width * 2, ImGui::GetCursorScreenPos().y),
		ImVec2(ImGui::GetCursorScreenPos().x + width * 3.f, ImGui::GetCursorScreenPos().y + height), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)gbuffer->gSpecular, ImVec2(ImGui::GetCursorScreenPos().x + width * 3, ImGui::GetCursorScreenPos().y),
		ImVec2(ImGui::GetCursorScreenPos().x + width * 4.f, ImGui::GetCursorScreenPos().y + height), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void DeferredShadingScene::ImGUI_Light()
{
	ImGui::NewLine();
	static const char* Lights[] =
	{ "Light#1",
	   "Light#2" ,
	   "Light#3",
	   "Light#4",
	   "Light#5",
	   "Light#6",
	   "Light#7",
	   "Light#8"
	};
	static  const char* current_light = Lights[0];
	static unsigned current = 0;
	if (ImGui::BeginCombo("Select Light", current_light)) // The second parameter is the label previewed before opening the combo.
	{
		for (unsigned n = 0; n < numLight; n++)
		{
			bool is_selected = (current_light == Lights[n]);
			if (ImGui::Selectable(Lights[n], is_selected))
			{
				current_light = Lights[n];
				current = n;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	float scale = lights[current]->GetScale().x;
	glm::vec3 color = lights[current]->GetDiffuseColor();
	if (ImGui::SliderFloat("Light Range", &scale,0,10))
	{
		lights[current]->SetScale(glm::vec3(scale));
	}
	if (ImGui::ColorEdit3("Light Color", &color.x))
	{
		lights[current]->SetDiffuse(color);
	}


	ImGui::Checkbox("Draw Local Light Range", &drawLightRange[current]);
}

void DeferredShadingScene::ImGUI_Model()
{
	ImGui::Begin("Setting");

	static const char* model[] = { "bunny_1", "bunny_2",  "bunny_3" };
	static const char* current_item = model[0];
	static int current = 0;
	if (ImGui::BeginCombo("Model", current_item)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < 3; n++)
		{
			bool is_selected = (current_item == model[n]);
			if (ImGui::Selectable(model[n], is_selected))
			{
				current_item = model[n];
				current = n;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	glm::vec3 color = objects[current]->GetDiffuseColor();
	if (ImGui::ColorEdit3("Diffuse Color", &color.x))
	{
		objects[current]->SetDiffuse(color);
	}
	color = objects[current]->GetSpecularColor();
	if (ImGui::ColorEdit3("Specular Color", &color.x))
	{
		objects[current]->SetSpecular(color);
	}
}

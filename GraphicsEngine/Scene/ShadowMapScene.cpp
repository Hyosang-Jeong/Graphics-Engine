#include "ShadowMapScene.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include"../Engine.h"
#include"Camera.h"
#include"RenderableObject.h"
#include"Rendering.h"
#include <GL/glew.h>

ShadowMapScene::ShadowMapScene():shadowMapping(1280,1280)
{
	camera = Engine::GetCamera();

	SetupObjects();
	SetupShader();

	lightPos = glm::vec3(0, 10, 10);
	lightView = glm::lookAt(lightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	lightColor = glm::vec3(1, 1, 1);

	block = BuildHammersleyPoints(block_N);

	glGenBuffers(1, &id);
	bindpoint = 1; // Increment this for other blocks.
	glBindBufferBase(GL_UNIFORM_BUFFER, bindpoint, id);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(block), &block, GL_STATIC_DRAW);
}

ShadowMapScene::~ShadowMapScene()
{
	delete background;
	for (auto& object : objects)
	{
		delete object;
	}
}

void ShadowMapScene::Update(float deltaTime)
{
	camera->Update(deltaTime);
	lightView = glm::lookAt(lightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void ShadowMapScene::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Shadow Pass
	ShadowPass();

	//blur
	shadowMapping.BlurPass(blurXShader, blurYShader);

	//deferred shading
	DeferredPass();

	//background
	ForwardPass();

	OnImGuiRender();
}

void ShadowMapScene::OnImGuiRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	if(ImGui::BeginMenu("Object Control") )
	{
		static const char* model[] = { "Plane", "obj_1",  "obj_2" };
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
		float roughness{ objects[current]->GetRoughness() / 100.f };
		float metalness{ objects[current]->GetMetalness() };
		if (ImGui::SliderFloat("roughness", &roughness, 1.f, 100.f))
		{
			objects[current]->SetRoughness(roughness * 100.f);
		}
		if (ImGui::SliderFloat("metalness", &metalness, 0.1f, 1.f))
		{
			objects[current]->SetMetalness(metalness);
		}

		if (ImGui::SliderInt("hammersley Block num", &block_N, 1, 40))
		{
			int kk;
			int pos = 0;

			for (int k = 0; k < block_N; k++)
			{
				kk = k;
				float u = 0.0f;
				for (float p = 0.5f; kk; p *= 0.5f, kk >>= 1)
				{
					if (kk & 1)
						u += p;
				}
				float v = (k + 0.5) / (float)block_N;
				block.hammersley[pos++] = u;
				block.hammersley[pos++] = v;
			}

			bindpoint = 1; // Increment this for other blocks.
			glBindBufferBase(GL_UNIFORM_BUFFER, bindpoint, id);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(block), &block, GL_STATIC_DRAW);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Light Control"))
	{
		ImGui::SliderFloat3("LightPosition", &lightPos[0], -10, 10);
		ImGui::ColorEdit3("Light Color", &lightColor[0]);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Drawing Control"))
	{
		const char* items[] = { "ALL", "Position", "Normal", "Diffuse", "Specular", "ShadowMap" };
		static int item_current = 0;
		if (ImGui::ListBox("Drawing Option", &item_current, items, IM_ARRAYSIZE(items), 6))
		{
			drawingIndex = item_current;
		}

		ImGui::SliderFloat("Exposure", &exposure, 0.1f, 5.f);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Shadow Control"))
	{
		ImGui::Checkbox("Use Blured ShadowMap", &useBlurTexture);

		if (ImGui::SliderInt("BlurWidth", &blurwidth, 1, 50))
		{
			shadowMapping.createGaussianKernel(blurwidth);
		}
		ImGui::NewLine();
		static bool light_view = false;
		if (ImGui::Checkbox("Scene from Light View",&light_view))
		{
			FromLightView = !FromLightView;
		}

		ImGui::EndMenu();
	}
	

}


void ShadowMapScene::SetupObjects()
{
	objects.push_back(new Quad());
	objects.push_back(new RenderableObject(Engine::GetModelManager().Load("../obj/teapot.obj")));
	objects.push_back(new RenderableObject(Engine::GetModelManager().Load("../obj/teapot.obj")));

	objects[1]->SetSpecular(glm::vec3(1, 1, 1));
	objects[2]->SetSpecular(glm::vec3(1, 1, 1));

	objects[1]->SetDiffuse(glm::vec3(0, 0, 0));
	objects[2]->SetDiffuse(glm::vec3(1, 1, 1));

	objects[1]->SetPosItion(glm::vec3(5, 0, 0));
	objects[2]->SetPosItion(glm::vec3(0, 0, 0));

	objects[1]->SetScale(glm::vec3(2.5, 2.5, 1.5));
	objects[2]->SetScale(glm::vec3(2.5, 2.5, 1.5));

	objects[1]->SetRoughness(10000.f);

	background = new Sphere();
	background->SetDiffuse("Newport_Loft_Ref.hdr");
	std::vector<glm::vec3> irr = calculate_Irr_coefficients(Engine::GetTextureManager().Load("Newport_Loft_Ref.hdr"));
	writeIrr_image("../textures/irradiance_map.irr.hdr", irr);
	background->SetScale({ 100,100,100 });



	objects[0]->SetRotation({ -90,0,0 });
	objects[0]->SetPosItion({ 0,-4,0 });
	objects[0]->SetScale({ 10,10,1 });
}

void ShadowMapScene::SetupShader()
{
	firstPassShader = Engine::GetShaderManager().Load("gBufferPass.vert", "gBufferPass.frag");
	secondPassShader = Engine::GetShaderManager().Load("lightingPass.vert", "lightingPass.frag");
	shadowShader = Engine::GetShaderManager().Load("shadow.vert", "shadow.frag");
	blurXShader = Engine::GetShaderManager().Load("blur_x.comp");
	blurYShader = Engine::GetShaderManager().Load("blur_y.comp");
	backgroundShader = Engine::GetShaderManager().Load("background.vert", "background.frag");
	fsqDebugShader = Engine::GetShaderManager().Load("FSQ_Debug.vert", "FSQ_Debug.frag");

}

void ShadowMapScene::ForwardPass()
{
	if (drawingIndex != 0)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsqDebugShader->active();
		GLint texture = glGetUniformLocation(fsqDebugShader->ID, "debugTexture");
		glUniform1i(texture, 0);
		glActiveTexture(GL_TEXTURE0);
		switch (drawingIndex)
		{
		case 1:
			glBindTexture(GL_TEXTURE_2D, deferredRendering.GetGbuffer()->gPosition);
			break;
		case 2:
			glBindTexture(GL_TEXTURE_2D, deferredRendering.GetGbuffer()->gNormal);
			break;
		case 3:
			glBindTexture(GL_TEXTURE_2D, deferredRendering.GetGbuffer()->gDiffuse);
			break;
		case 4:
			glBindTexture(GL_TEXTURE_2D, deferredRendering.GetGbuffer()->gSpecular);
			break;
		case 5:
			if(!useBlurTexture)
				glBindTexture(GL_TEXTURE_2D, shadowMapping.GetShadowMap());
			else
				glBindTexture(GL_TEXTURE_2D, shadowMapping.GetBluredShadowMap());
			break;
		default:
			break;
		}
		fsq.Rendering(fsqDebugShader);
	}
	else
	{
		deferredRendering.CopyDepthInfo();
		glCullFace(GL_FRONT);
		backgroundShader->active();
		backgroundShader->setMat4("projection", projection);
		backgroundShader->setMat4("view", glm::mat4(camera->GetView()));
		backgroundShader->setFloat("exposure", exposure);
		fowardRendering.Render({ background }, backgroundShader);
		glCullFace(GL_BACK);
	}
}

void ShadowMapScene::ShadowPass()
{
	shadowShader->active();
	shadowShader->setFloat("near", light_near);
	shadowShader->setFloat("far", light_far);
	shadowMapping.ShadowPass(objects, shadowShader, projection, lightView);
}

void ShadowMapScene::DeferredPass()
{
	firstPassShader->active();
	firstPassShader->setMat4("projection", projection);
	if(!FromLightView)
		firstPassShader->setMat4("view", camera->GetView());
	else
		firstPassShader->setMat4("view", lightView);

	secondPassShader->active();
	secondPassShader->setVec3("eyePosition", camera->GetPosition());
	secondPassShader->setVec3("lightPosition", lightPos);
	secondPassShader->setVec3("lightColor", lightColor);
	secondPassShader->setMat4("lightSpaceMatrix", projection * lightView);
	secondPassShader->setFloat("near", light_near);
	secondPassShader->setFloat("far", light_far);
	secondPassShader->setFloat("N", block_N);
	secondPassShader->setFloat("exposure", exposure);


	int loc = glGetUniformBlockIndex(secondPassShader->ID, "HammersleyBlock");
	glUniformBlockBinding(secondPassShader->ID, loc, bindpoint);


	if(useBlurTexture)
		deferredRendering.useShadow(shadowMapping.GetBluredShadowMap());
	else
		deferredRendering.useShadow(shadowMapping.GetShadowMap());

	glUniform1i(glGetUniformLocation(secondPassShader->ID, "sky"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, Engine::GetTextureManager().Load("Newport_Loft_Ref.hdr").id);	
	
	glUniform1i(glGetUniformLocation(secondPassShader->ID, "sky_irr"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, Engine::GetTextureManager().Load("irradiance_map.irr.hdr").id);



	deferredRendering.Render(objects, firstPassShader, secondPassShader);
}


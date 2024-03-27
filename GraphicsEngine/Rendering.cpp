#include "Rendering.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#define MAX_BLUR_WIDTH 50
#define WEIGHTS_SIZE (2 * MAX_BLUR_WIDTH + 1)

void ForwardRendering::Render(std::vector<RenderableObject*> renderable, Shader* shader)
{
	for (auto& obj : renderable)
	{
		obj->Rendering(shader);
	}
}

void DeferredRendering::Render(std::vector<RenderableObject*> renderable, Shader* firstPass, Shader* secondPass)
{
	//G-buffer pass

	gbuffer.Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& obj : renderable)
	{
		obj->Rendering(firstPass);
	}

	gbuffer.UnUse();

	//Light pass
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	secondPass->active();

	GLint texture = glGetUniformLocation(secondPass->ID, "gPosition");
	glUniform1i(texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);

	texture = glGetUniformLocation(secondPass->ID, "gNormal");
	glUniform1i(texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);

	texture = glGetUniformLocation(secondPass->ID, "gDiffuse");
	glUniform1i(texture, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuffer.gDiffuse);

	texture = glGetUniformLocation(secondPass->ID, "gSpecular");
	glUniform1i(texture, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuffer.gSpecular);

	if (useshadow == true)
	{
		texture = glGetUniformLocation(secondPass->ID, "gShadowMap");
		glUniform1i(texture, 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	}

	FSQ.Rendering(secondPass);

}

void DeferredRendering::LocalLightsPass(std::vector<RenderableObject*> renderable, Shader* localLight)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	localLight->active();

	GLint texture = glGetUniformLocation(localLight->ID, "gPosition");
	glUniform1i(texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);

	texture = glGetUniformLocation(localLight->ID, "gNormal");
	glUniform1i(texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);

	texture = glGetUniformLocation(localLight->ID, "gDiffuse");
	glUniform1i(texture, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuffer.gDiffuse);

	texture = glGetUniformLocation(localLight->ID, "gSpecular");
	glUniform1i(texture, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuffer.gSpecular);


	for (auto& light : renderable)
	{
		Sphere* sphere = reinterpret_cast<Sphere*>(light);
		localLight->setVec3("lightPosition", light->GetPostion());
		localLight->setFloat("radius", sphere->GetRadius());
		light->Rendering(localLight);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void DeferredRendering::CopyDepthInfo()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.Gethandle());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(
		0, 0, gbuffer.textureWidth, gbuffer.textureHeight, 0, 0, gbuffer.textureWidth, gbuffer.textureHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRendering::useShadow(unsigned shadowTexture)
{
	shadowMapTexture = shadowTexture;
	useshadow = true;
}

 const G_buffer* DeferredRendering::GetGbuffer()
{
	return &gbuffer;
}


 ShadowMapping::ShadowMapping(unsigned shadowWidth_, unsigned shadowHeight_)
 {
	 shadowWidth = shadowWidth_;
	 shadowHeight = shadowHeight_;
	 shadowMap.Use();
	 glGenTextures(1, &shadowMapTexture);
	 glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, shadowWidth, shadowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 shadowMap.AttachTexture(shadowMapTexture, 0);
	 shadowMap.UnUse();

	 glGenTextures(1, &blur_xTexture);
	 glBindTexture(GL_TEXTURE_2D, blur_xTexture);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, shadowWidth, shadowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 glGenTextures(1, &blur_yTexture);
	 glBindTexture(GL_TEXTURE_2D, blur_yTexture);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, shadowWidth, shadowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	 createGaussianKernel(5);
 }

 void ShadowMapping::ShadowPass(std::vector<RenderableObject*> renderable,Shader* shader, glm::mat4 ligthProjection, glm::mat4 lightView)
 {
	glViewport(0, 0, shadowWidth, shadowHeight);
	 shadowMap.Use();
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 shader->active();
	 shader->setMat4("projection", ligthProjection);
	 shader->setMat4("view", lightView);

	 for (auto& obj : renderable)
	 {
		 obj->Rendering(shader);
	 }

	 shadowMap.UnUse();
	 glViewport(0, 0, Engine::GetWindowSize().x, Engine::GetWindowSize().y);
 }

 void ShadowMapping::BlurPass(Shader* blurXShader, Shader* blurYShader)
 {
	 blurXShader->active();

	 glBindImageTexture(0, shadowMapTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	 blurXShader->setInt("src", 0);
	 glBindImageTexture(1, blur_xTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	 blurXShader->setInt("dst", 1);

	 glGenBuffers(1, &bufferID);
	 glBindBuffer(GL_UNIFORM_BUFFER, bufferID);

	 GLint loc = glGetUniformBlockIndex(blurXShader->ID, "blurKernel");
	 glUniformBlockBinding(blurXShader->ID, loc, 0);

	 glBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferID);
	 glBufferData(GL_UNIFORM_BUFFER, kernel.size() * sizeof(float), kernel.data(), GL_STATIC_DRAW);

	 blurXShader->setInt("blurWidth", blurwidth);
	 blurXShader->setInt("imageWidth", shadowWidth);

	 glDispatchCompute(shadowWidth / 128, shadowHeight, 1);

	 glBindBuffer(GL_UNIFORM_BUFFER, 0);
	 glDeleteBuffers(1, &bufferID);

	 //x + y blur
	 blurYShader->active();

	 glBindImageTexture(0, blur_xTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	 blurYShader->setInt("src", 0);
	 glBindImageTexture(1, blur_yTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	 blurYShader->setInt("dst", 1);

	 glGenBuffers(1, &bufferID);
	 glBindBuffer(GL_UNIFORM_BUFFER, bufferID);

	 loc = glGetUniformBlockIndex(blurYShader->ID, "blurKernel");
	 glUniformBlockBinding(blurYShader->ID, loc, 0);

	 glBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferID);
	 glBufferData(GL_UNIFORM_BUFFER, kernel.size() * sizeof(float), kernel.data(), GL_STATIC_DRAW);

	 blurYShader->setInt("blurWidth", blurwidth);
	 blurYShader->setInt("imageHeight", shadowHeight);

	 glDispatchCompute(shadowWidth, shadowHeight / 128, 1);

	 glBindBuffer(GL_UNIFORM_BUFFER, 0);
	 glDeleteBuffers(1, &bufferID);
 }

 unsigned ShadowMapping::GetShadowMap()
 {
	 return shadowMapTexture;
 }

 unsigned ShadowMapping::GetBluredShadowMap()
 {
	 return blur_yTexture;
 }

 void ShadowMapping::createGaussianKernel(int blurWidth)
 {
	 int size = WEIGHTS_SIZE;
	 std::vector<float> new_kernel(size, 0);
	 float sum = 0.0;
	 float s = blurWidth / 2.f;
	 for (int i = -blurWidth; i <= blurWidth; i++) {
		 float value = std::exp(-0.5f * (i / s) * (i / s));
		 new_kernel[MAX_BLUR_WIDTH - i] = value;
		 sum += value;
	 }

	 for (int i = 0; i < size; i++) {
		 new_kernel[i] /= sum;
	 }
	 kernel = new_kernel;
	 blurwidth = blurWidth;
 }
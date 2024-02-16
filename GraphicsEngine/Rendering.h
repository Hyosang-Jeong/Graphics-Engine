#pragma once
#include"RenderableObject.h"
#include"G-buffer.h"
#include<vector>
class ForwardRendering {
public:
    void Render(std::vector<RenderableObject*> renderable, Shader* shader) ;
};

class DeferredRendering  {
public:
    void Render(std::vector<RenderableObject*> renderable, Shader* firstPass, Shader* secondPass) ;
    void LocalLightsPass(std::vector<RenderableObject*> renderable, Shader* localLight);
    void CopyDepthInfo();
    void useShadow(unsigned shadowTexture);
    const G_buffer* GetGbuffer();
private:
    G_buffer gbuffer;
    Quad FSQ;
    unsigned shadowMapTexture;
    bool useshadow{ false };
};

class ShadowMapping {
public:
    ShadowMapping(unsigned shadowWidth, unsigned shadowHeight);
    void ShadowPass(std::vector<RenderableObject*> renderable,Shader* shader, glm::mat4 ligthProjection, glm::mat4 lightView);
    void BlurPass(Shader* blur_x_shader, Shader* blur_y_shader);
    unsigned GetShadowMap();
    unsigned GetBluredShadowMap();
    void createGaussianKernel(int blurWidth);
private:
    FBO shadowMap;
    unsigned bufferID;
    unsigned shadowWidth;
    unsigned shadowHeight;
    unsigned shadowMapTexture;
    unsigned  blur_xTexture;
    unsigned  blur_yTexture;
    int blurwidth;
    std::vector<float> kernel;
};

#include"Engine.h"
#include"../GraphicsEngine/Scene/DeferredShadingScene.h"
#include"../GraphicsEngine/Scene/ShadowMapScene.h"
int main()
{
    Engine& engine = Engine::Instance();
    engine.init(1600, 1000);
    //DeferredShadingScene* simple_render = new DeferredShadingScene();
    ShadowMapScene* shadowMap = new ShadowMapScene();
    engine.GetSceneManager().AddScene(shadowMap);
    while (!engine.ShouldClose())
    {
        engine.Update();
        engine.Draw();
    }

    return 0;
}
# Graphics Engine

![image](https://github.com/Hyosang-Jeong/Graphics-Engine/assets/70678504/838de608-be37-41c0-a479-a2ca80fd97ec)


# Features

* **Deferred Shading**: Implemented deferred shading in the graphics engine. This advanced rendering technique allows for more efficient handling of lighting calculations, especially in scenes with multiple light sources, by deferring the computation of lighting until after all objects are rendered.

* **Hybrid Rendering Capability**: Integrated both deferred shading and forward rendering techniques into the engine, providing a hybrid rendering approach. This hybrid model enables the flexibility and strengths of both methods, offering enhanced rendering capabilities for various types of scenes and materials.

* **Shadow Map**: Developed a shadow map feature to add realistic shadows to 3D objects. This technique involves creating a depth map from the light's perspective, which is then used to calculate and render shadows accurately on the 3D models in the scene.

* **Gaussian Blur with Compute Shaders**: Implemented a fixed-width blur using a convolution filter with a Gaussian weight kernel, utilizing compute shaders. This method efficiently blurs images, providing a smoother appearance, and is particularly effective in creating realistic soft shadows and depth-of-field effects.

* **Moment Shadow Map with Blurred Filter**: Extended the shadow mapping technique by implementing a moment shadow map using a blurred filter. This approach enhances the quality of shadows, reducing artifacts and providing softer, more realistic shadows, especially in scenes with complex lighting.

* **Physically-Based Lighting(PBL) and Image-Based Lighting(IBL)**: Implemented Physically-Based Lighting (PBL) for lifelike material interactions and Image-Based Lighting (IBL) for enhanced environmental reflections, significantly elevating the engine's visual fidelity.

* **3D Model Loading with Assimp**: Integrated the Assimp library to enable efficient loading and processing of various 3D model formats. This feature allows for seamless importing of models, including their textures and animations, supporting a wide range of 3D assets for use in the engine.

# Control

* **Camera Control**: WASD to move, QEZC to rotate camera

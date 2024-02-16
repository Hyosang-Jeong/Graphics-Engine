#include "Light.h"

Light::Light(LIGHT_TYPE type, Material material, glm::vec3 pos, glm::vec3 spot):
	type(type),material(material),position(pos),Inner_Outer_Falloff(spot),direction(0)
{
}

void Light::UpdatePosition(glm::vec3 pos)
{
    position = pos;
}

void Light::UpdateAmbient(glm::vec3 ambient)
{
    material.ambient = ambient;
}

void Light::UpdateDiffuse(glm::vec3 diffuse)
{
    material.diffuse = diffuse;
}

void Light::UpdateSpecular(glm::vec3 specular)
{
    material.specular = specular;
}

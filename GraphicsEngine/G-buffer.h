#ifndef GBUFFER_H
#define GBUFFER_H
#include <GL/glew.h> 
#include"FBO.h"
class G_buffer
{
public:
	G_buffer();
	~G_buffer();
	void GenerateTextures(unsigned width, unsigned height);
	void Use();
	void UnUse();
	GLuint Gethandle();
	FBO fbo;
	unsigned int gPosition, gNormal, gDiffuse,gSpecular, rboDepth;
	unsigned textureWidth;
	unsigned textureHeight;
};
#endif
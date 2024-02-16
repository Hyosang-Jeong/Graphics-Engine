#include "FBO.h"

FBO::FBO()
{
	GenerateFBO();
}

FBO::~FBO()
{
	glDeleteFramebuffers(1, &fbo);
}

void FBO::GenerateFBO()
{
	glGenFramebuffers(1, &fbo); // create the FB object handle
}

void FBO::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
void FBO::UnUse()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FBO::AttachTexture(unsigned int texture, unsigned index)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, texture, 0);
}

void FBO::AttachDepthTexture(unsigned int texture)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

}

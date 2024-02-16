#ifndef FBO_H
#define FBO_H
#include <GL/glew.h> 


class FBO
{
public:
	FBO();
	~FBO();
	void GenerateFBO();
	void Use();
	void UnUse();
	void AttachTexture(unsigned int texture, unsigned index);
	void AttachDepthTexture(unsigned int texture);

	GLuint fbo;
};
#endif
#include "Material.h"
#include "stdafx.h"


void Material::setMaterial(unsigned int prog)
{
	GLint diffTexLoc = glGetUniformLocation(prog, "diffTex");

	glUniform1i(diffTexLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, _diffTex);
}
#include "texture.h"

using namespace blowgun;

Texture::Texture(GLenum target, u32 name)
	: target_(target), name_(name)
{
}

void
Texture::Bind() const
{
	glBindTexture(target_, name_);
}

void
Texture::Delete() const
{
	glDeleteTextures(1, &name_);
}

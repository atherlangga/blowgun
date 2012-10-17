#ifndef BLOWGUN_TEXTURE_H_
#define BLOWGUN_TEXTURE_H_

#include <GLES2/gl2.h>

#include "types.h"

namespace blowgun
{

/**
 * Texture is handle to data that will be used by
 * the OpenGL implementation to draw something to
 * the screen.
 *
 * In most cases, the data mentioned above will be
 * read from image file.
 */
class Texture
{
private:
	const GLenum target_;
	const u32 name_;

private:
	explicit Texture(GLenum target, u32 name);

	Texture(const Texture &);// = delete;
	Texture & operator=(const Texture &);// = delete;

private:
	friend class TextureBuilder;

public:

	/**
	 * Bind the texture to its designated target.
	 */
	void Bind() const;

	/**
	 * Delete texture data stored on the GPU.
	 */
	void Delete() const;

};

}

#endif // BLOWGUN_TEXTURE_H_

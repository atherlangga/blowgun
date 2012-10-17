#ifndef BLOWGUN_TEXTURE_BUILDER_H_
#define BLOWGUN_TEXTURE_BUILDER_H_

#include <vector>
#include <map>
#include <memory>

#include <GLES2/gl2.h>

#include "types.h"

namespace blowgun
{

class Texture;

class TextureBuilder
{
private:
	GLenum target_;
	u8 level_of_detail_;
	GLenum format_;
	GLenum type_;
	u32 width_;
	u32 height_;
	std::vector<byte> data_;
	std::map<GLenum, u32> params_;

public:
	explicit TextureBuilder();
	TextureBuilder & SetTarget(GLenum target);
	TextureBuilder & SetLevelOfDetail(u8 level_of_detail);
	TextureBuilder & SetFormat(GLenum format);
	TextureBuilder & SetWidth(u32 width);
	TextureBuilder & SetHeight(u32 height);
	TextureBuilder & SetType(GLenum type);
	TextureBuilder & SetData(std::vector<byte> data);
	TextureBuilder & AddParameter(GLenum param_name, u32 param_value);
	std::unique_ptr<Texture> Build();
};

}

#endif // BLOWGUN_TEXTURE_BUILDER_H_

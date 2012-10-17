#include "texture_builder.h"

#include "texture.h"

using namespace blowgun;

TextureBuilder::TextureBuilder() :
	target_(), level_of_detail_(), format_(),
	type_(), width_(), height_(), data_(), params_()
{
}

TextureBuilder &
TextureBuilder::SetTarget(GLenum target)
{
	// TODO: ASSERT target
	target_ = target;
	return *this;
}

TextureBuilder &
TextureBuilder::SetLevelOfDetail(u8 level_of_detail)
{
	// TODO: ASSERT level of detail
	level_of_detail_ = level_of_detail;
	return *this;
}

TextureBuilder &
TextureBuilder::SetFormat(GLenum format)
{
	// TODO: ASSERT format
	format_ = format;
	return *this;
}

TextureBuilder &
TextureBuilder::SetWidth(u32 width)
{
	// TODO: ASSERT width
	width_ = width;
	return *this;
}

TextureBuilder &
TextureBuilder::SetHeight(u32 height)
{
	// TODO: ASSERT height
	height_ = height;
	return *this;
}

TextureBuilder &
TextureBuilder::SetType(GLenum type)
{
	// TODO: ASSERT type
	type_ = type;
	return *this;
}

TextureBuilder &
TextureBuilder::SetData(std::vector<byte> data)
{
	data_ = data;
	return *this;
}

TextureBuilder &
TextureBuilder::AddParameter(GLenum param_name, u32 param_value)
{
	params_[param_name] = param_value;
	return *this;
}

std::unique_ptr<Texture>
TextureBuilder::Build()
{
	// TODO: ASSERT every parameters

	// Generate a name for the texture
	u32 name;
	glGenTextures(1, &name);

	// Bind the texture to the target
	glBindTexture(target_, name);

	// Now that texture is bound, set the parameters. All
	// the parameters are kept as key-value pair.
	for (auto i = params_.begin(); i != params_.end(); ++i)
	{
		// Get the key: parameter name
		GLenum param_name = i->first;
		// Get its value
		u32 param_value = i->second;

		// Upload the name and the value at once.
		glTexParameteri(target_, param_name, param_value);
	}

	// Last, upload the texture to GPU.
	glTexImage2D(target_, level_of_detail_, format_,
		width_, height_, 0, format_, type_, &data_[0]);

	return std::unique_ptr<Texture>(new Texture(target_, name));
}

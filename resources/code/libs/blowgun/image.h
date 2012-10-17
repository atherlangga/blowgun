#ifndef BLOWGUN_IMAGE_H_
#define BLOWGUN_IMAGE_H_

#include "types.h"

#include <vector>

namespace blowgun
{

struct Image
{
	const u16                width;
	const u16                height;
	const byte               bpp;
	const std::vector<byte>  data;

	explicit Image(u16 width, u16 height, byte bpp, std::vector<byte> data) :
		width(width), height(height), bpp(bpp), data(data) {}
};

}

#endif

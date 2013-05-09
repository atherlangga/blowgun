#include "image_loader_tga.h"

#include <stdexcept>

#include "types.h"

using namespace blowgun;

// File-scope utility declaration
namespace
{
// The usage of `#pragma pack` is necessary to ensure the compiler
// doesn't do struct padding.
#pragma pack(push, 1)
	/**
	 * The standard TGA Header file definition.
	 */
	struct TGA_HEADER
	{
		byte id_size;
		byte colour_map_type;
		byte image_type;
		u16  palette_start;
		u16  palette_size;
		byte palette_entry_depth;
		u16  x;
		u16  y;
		u16  width;
		u16  height;
		byte bits_per_pixel;
		byte image_descriptor;
	};

	/**
	 * The standard TGA color order.
	 */
	struct TGA_RGB_VALUE
	{
		byte blue;
		byte green;
		byte red;
	};
#pragma pack(pop)
}

bool
ImageLoaderTGA::CanLoad(std::istream & /*input*/) const
{
	// TODO: Read the header of the input stream to ensure
	// that the stream conforms to TGA spec.
	return true;
}

std::shared_ptr<Image>
ImageLoaderTGA::Load(std::istream & input) const
{
	const u32 kInvertedBitInfoLocation = 1 << 5;

	input.seekg(0, std::ios::beg);

	/*
	 * Read the header first.
	 */
	TGA_HEADER image_info;
	input.read(reinterpret_cast<char *>(&image_info), sizeof(TGA_HEADER));

	/*
	 * Then the actual content.
	 */
	// TODO: This code only supports RGB at the moment.
	u32 pixels_count = image_info.width * image_info.height;
	u32 content_size = sizeof(TGA_RGB_VALUE) * pixels_count;
	
	TGA_RGB_VALUE * raw_pixels = new TGA_RGB_VALUE[pixels_count];
	input.read(reinterpret_cast<char *>(raw_pixels), content_size);
	if ((u32) input.gcount() != content_size)
	{
		std::cerr << "Read bytes count differs from content size!" << std::endl;
	}

	/*
	 * Process the content.
	 */
	bool value_is_inverted =
		(image_info.image_descriptor & kInvertedBitInfoLocation) == kInvertedBitInfoLocation;
	std::vector<byte> pixels(pixels_count * 3);
	u32 i = 0;
	for (u16 y = 0; y < image_info.height; y++)
	{
		for(u16 x = 0; x < image_info.width; x++)
		{
			u32 index = y * image_info.width + x;

			// If this TGA stream is _not_ inverted, adjust the index.
			if(!value_is_inverted)
				index = (image_info.height - 1 - y) * image_info.width + x;

			pixels[(i * 3) + 0] = raw_pixels[index].red;
			pixels[(i * 3) + 1] = raw_pixels[index].green;
			pixels[(i * 3) + 2] = raw_pixels[index].blue;

			i++;
		}
	}

	delete[] raw_pixels;

	return std::make_shared<Image>(
		image_info.width,
		image_info.height,
		image_info.bits_per_pixel,
		pixels
	);
}

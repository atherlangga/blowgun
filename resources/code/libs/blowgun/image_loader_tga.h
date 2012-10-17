#ifndef BLOWGUN_IMAGE_LOADER_TGA_H_
#define BLOWGUN_IMAGE_LOADER_TGA_H_

#include "image_loader.h"

namespace blowgun
{

class ImageLoaderTGA : public ImageLoader
{
public:
	virtual bool CanLoad(std::istream & input) const;
	virtual std::shared_ptr<Image> Load(std::istream & input) const;
};

}

#endif

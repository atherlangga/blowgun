#ifndef BLOWGUN_IMAGE_LOADER_H_
#define BLOWGUN_IMAGE_LOADER_H_

#include "image.h"

#include <iostream>
#include <memory>

namespace blowgun
{

class ImageLoader
{
public:
	virtual bool CanLoad(std::istream & input) const = 0;
	virtual std::shared_ptr<Image> Load(std::istream & input) const = 0;
	virtual ~ImageLoader() {};
};

}

#endif

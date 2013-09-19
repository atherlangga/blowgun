#ifndef BLOWGUN_VERTICES_BUILDER_H
#define BLOWGUN_VERTICES_BUILDER_H

#include <string>
#include <memory>

#include "types.h"
#include "vertices.h"

namespace blowgun
{

class VerticesLayout
{
public:
	void PlaceAttribute(const u32 position, const std::string name);

private:
	std::map<u32, std::string> attributes_mapping_;

friend class VerticesBuilder;
};

class VerticesBuilder
{
public:
	explicit VerticesBuilder(const VerticesLayout layout);

	VerticesBuilder & AddAtrribute(const std::string name, void* data);
	std::unique_ptr<Vertices> Build(const VerticesFormat::Enum format);

private:
	const VerticesLayout layout_;
};

}

#endif // BLOWGUN_VERTICES_BUILDER_H

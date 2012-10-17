#include "model.h"

using namespace blowgun;

Model::Model() :
	vertex_attributes_(std::vector<VertexAttribute>())
{
}

Model::Model(std::vector<VertexAttribute> vertex_attributes) :
	vertex_attributes_(vertex_attributes)
{
}

std::vector<VertexAttribute> const
Model::vertex_attributes() const
{
	return vertex_attributes_;
}

void
Model::AddVertexAttribute(VertexAttribute va)
{
	vertex_attributes_.push_back(va);
}

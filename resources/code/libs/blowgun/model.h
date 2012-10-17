#ifndef BLOWGUN_MODEL_H_
#define BLOWGUN_MODEL_H_

#include <vector>

#include "vertex_attribute.h"

namespace blowgun
{

class Model
{
private:
	/**
	 * Container to hold `VertexAttribute` for the `Model`.
	 */
	std::vector<VertexAttribute> vertex_attributes_;

	// Disallow copy and assign.
	Model(const Model & rhs);
	Model & operator=(const Model & rhs);

public:
	Model();
	Model(std::vector<VertexAttribute> vertex_attributes);

	std::vector<VertexAttribute> const vertex_attributes() const;

	void AddVertexAttribute(VertexAttribute va);
};

}

#endif // of BLOWGUN_MODEL_H_

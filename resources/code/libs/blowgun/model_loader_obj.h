#ifndef BLOWGUN_MODEL_LOADER_OBJ_H_
#define BLOWGUN_MODEL_LOADER_OBJ_H_

#include <string>
#include <memory>

#include "model.h"

namespace blowgun
{

class ModelLoaderOBJ
{
public:
    explicit ModelLoaderOBJ();
    std::shared_ptr<Model> Load(std::istream & stream);

private:
	ModelLoaderOBJ(const ModelLoaderOBJ &); // = delete;
    ModelLoaderOBJ & operator=(const ModelLoaderOBJ &); // = delete;
};

}

#endif // BLOWGUN_MODEL_LOADER_OBJ_H_

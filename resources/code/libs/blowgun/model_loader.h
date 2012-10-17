#ifndef BLOWGUN_MODEL_LOADER_H_
#define BLOWGUN_MODEL_LOADER_H_

namespace blowgun
{

class ModelLoader
{
public:
	virtual bool CanLoad(std::istream & input) = 0;
	virtual std::shared_ptr<Model> Load(std::istream & input) = 0;
	virtual ~ModelLoader() {};
};

}

#endif // BLOWGUN_MODEL_LOADER_H_

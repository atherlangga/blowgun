#include "../application.h"

class ModelLoadingApplication: public blowgun::Application
{
public:
	void OnInitialization();
	void OnDraw() const;
	void OnUpdate() {}
	void OnDestroy();
};

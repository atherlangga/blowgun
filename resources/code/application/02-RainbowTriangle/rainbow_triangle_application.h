#include "../application.h"

class RainbowTriangleApplication: public blowgun::Application
{
public:
	void OnInitialization();
	void OnDraw() const;
	void OnUpdate() {}
	void OnDestroy();
};

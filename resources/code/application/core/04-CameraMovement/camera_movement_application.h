#include "../application.h"

class CameraMovementApplication: public blowgun::Application
{
public:
	void OnInitialization();
	void OnDraw() const;
	void OnUpdate();
	void OnDestroy();
};

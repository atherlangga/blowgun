#include "../application.h"

/**
 * Application that will show a triangle on the screen.
 *
 * This is the "Hello World" application. This application
 * demonstrates basic usage of `blowgun::Program` and its
 * brother, `blowgun::ProgramBuilder`.
 */
class TriangleApplication: public blowgun::Application
{
public:
    void OnInitialization();
    void OnDraw() const;
    void OnUpdate() {} // Doesn't need `OnUpdate()` yet.
    void OnDestroy();
};

#ifndef BLOWGUN_APPLICATION_H_
#define BLOWGUN_APPLICATION_H_

namespace blowgun
{

/**
 * Base class for Application.
 * This class have to be a pure virtual abstract class to prevent
 * someone instantiate this class rather than its successor.
 */
class Application
{
public:
	/**
	 * Callback that will be called right before entering the main loop.
	 */
	virtual void OnInitialization() = 0;

	/**
	 * Callback for each frame.
	 */
	virtual void OnUpdate() = 0;

	/**
	 * Callback dispatched from system to draw the Application.
	 */
	virtual void OnDraw() const = 0;

	/**
	 * Callback that will be called shortly after main loop.
	 */
	virtual void OnDestroy() = 0;

	virtual ~Application() { };
};

}

#endif //BLOWGUN_APPLICATION_H_

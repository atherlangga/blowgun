#ifndef BLOWGUN_APPLICATION_FACTORY_H_
#define BLOWGUN_APPLICATION_FACTORY_H_

#include <memory>

#include "application.h"

namespace blowgun
{

/**
 * Factory for the Application.
 *
 * The client code must ensure that implementation for `ApplicationFactory`
 * present in client's layer. The OS-specific platform bootstrapper, in turn,
 * will use the implementation to start client's core application.
 */
class ApplicationFactory
{
public:
	/**
	 * Create the Application.
	 */
	std::shared_ptr<Application> CreateApplication();

	~ApplicationFactory() {};
};

}

#endif //BLOWGUN_APPLICATION_FACTORY_H_

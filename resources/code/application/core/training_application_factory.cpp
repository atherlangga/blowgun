#include <iostream>
#include <memory>
#include <stdexcept>

#include "application_factory.h"
#include "01-Triangle/triangle_application.h"
#include "02-RainbowTriangle/rainbow_triangle_application.h"
#include "03-ModelLoading/model_loading_application.h"
#include "04-CameraMovement/camera_movement_application.h"

using namespace blowgun;

std::shared_ptr<blowgun::Application> ApplicationFactory::CreateApplication()
{
	int selection = 0;

	while (selection < 1 || selection > 4)
	{
		std::cout << "Select application: " << std::endl;
		std::cout << "1. Triangle Application" << std::endl;
		std::cout << "2. Rainbow Triangle Application" << std::endl;
		std::cout << "3. Model Loading Application" << std::endl;
		std::cout << "4. Camera Movement Application" << std::endl;

		std::cin >> selection;
	}

	if (selection == 1)
	{
		return std::make_shared<TriangleApplication>(
			TriangleApplication());
	}
	else if (selection == 2)
	{
		return std::make_shared<RainbowTriangleApplication>(
			RainbowTriangleApplication());
	}
	else if (selection == 3)
	{
		return std::make_shared<ModelLoadingApplication>(
			ModelLoadingApplication());
	}
	else if (selection == 4)
	{
		return std::make_shared<CameraMovementApplication>(
			CameraMovementApplication());
	}

	throw new std::runtime_error("This line should never be reached.");
}

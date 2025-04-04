#pragma once
#include <string>
#include "ArcballCamera.h"

class Camera;
//A rather simple Factory using the base class Camera
//generates a Camera based on its type
class CameraFactory
{
public:

	static Camera* makeNewCam(std::string type);
};
#include "CameraFactory.h"
#include "Camera.h"
#include "ArcballCamera.h"
#include "IsometricCamera.h"
#include "FirstPersonCamera.h"
#include <assert.h>

using std::string;

Camera* CameraFactory::makeNewCam(string _type)
{
	printf("CAM TYPE: %s \n", _type.c_str());

	if (_type == "Arcball")
	{
		return new ArcballCamera();
	}
	else if (_type == "Isometric")
	{
		return new IsometricCamera();
	}
	else if (_type == "FirstPerson")
	{
		return new FirstPersonCamera();
	}
	else
	{
		printf("UNKNOWN CAMERA TYPE!");
		assert(0);
		return nullptr;
	}
}

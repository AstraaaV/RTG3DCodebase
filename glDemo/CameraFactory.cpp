#include "CameraFactory.h"
#include "Camera.h"
#include "ArcballCamera.h"
#include <assert.h>

using std::string;

Camera* CameraFactory::makeNewCam(string _type)
{
	printf("CAM TYPE: %s \n", _type.c_str());
	if (_type == "CAMERA")
	{
		return new Camera();
	}
	else if (_type == "Arcball")
	{
		return new ArcballCamera();
	}
	else if (_type == "Isometric")
	{
		return new Camera();
	}
	else if (_type == "FirstPerson")
	{
		return new Camera();
	}
	else
	{
		printf("UNKNOWN CAMERA TYPE!");
		assert(0);
		return nullptr;
	}
}

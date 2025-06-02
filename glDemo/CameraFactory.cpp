#include "CameraFactory.h"
#include "Camera.h"
#include "ArcballCamera.h"
#include "IsometricCamera.h"
#include "FirstPersonCamera.h"
#include <iostream>
#include <assert.h>

using std::string;
using std::cout;
using std::endl;

Camera* CameraFactory::makeNewCam(const string& type)
{
	cout << "[CAMERAFACTORY] Requested Cam Type: " << type << endl;

	if (type == "Arcball")
	{
		return new ArcballCamera();
	}
	else if (type == "Isometric")
	{
		return new IsometricCamera();
	}
	else if (type == "FirstPerson")
	{
		return new FirstPersonCamera();
	}
	else
	{
		cout << "[CAMERAFACTORY] Error. Unknown cam type: " << type << endl;
		assert(false && "Unknown camera type passed to CameraFactory.");
		return nullptr;
	}
}

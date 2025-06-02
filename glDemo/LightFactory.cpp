#include "LightFactory.h"
#include "Light.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include <iostream>
#include <assert.h>

using std::string;
using std::cout;
using std::endl;

Light* LightFactory::makeNewLight(const string& type)
{
	cout << "[LIGHTFACTORY] Requested Light Type: " << type << endl;

	if (type == "LIGHT")
	{
		return new Light();
	}
	else if (type == "DIRECTIONAL")
	{
		return new DirectionLight();
	}
	else if (type == "POINT")
	{
		return new PointLight();
	}
	else
	{
		cout << "[LIGHTFACTORY] Error. Unknown Light Type: " << type << endl;
		assert(false && "Unknown light type passed to LightFactory.");
		return nullptr;
	}
}

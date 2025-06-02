#include "LightFactory.h"
#include <assert.h>
#include "Light.h"
#include "DirectionLight.h"

Light* LightFactory::makeNewLight(std::string _type)
{
	printf("LIGHT TYPE: %s \n", _type.c_str());
	if (_type == "LIGHT")
	{
		return new Light();
	}
	else if (_type == "DIRECTION")
	{
		return new DirectionLight();
	}
	else
	{
		printf("UNKNOWN LIGHT TYPE!");
		assert(0);
		return nullptr;
	}
}
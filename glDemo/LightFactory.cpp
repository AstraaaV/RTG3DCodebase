#include "LightFactory.h"
#include <assert.h>
#include "Light.h"
#include "DirectionLight.h"
#include "PointLight.h"

Light* LightFactory::makeNewLight(std::string _type)
{
	printf("LIGHT TYPE: %s \n", _type.c_str());
	if (_type == "LIGHT")
	{
		return new Light();
	}
	else if (_type == "DIRECTIONAL")
	{
		return new DirectionLight();
	}
	else if (_type == "POINT")
	{
		return new PointLight();
	}
	else
	{
		printf("UNKNOWN LIGHT TYPE: %s\n", _type.c_str());
		assert(0);
		return nullptr;
	}
}

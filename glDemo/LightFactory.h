#pragma once
#include <string>
#include "PointLight.h"
#include "DirectionLight.h"

class Light;

//ditto for the other factories but now for lights!
class LightFactory
{
public:

	static Light* makeNewLight(const string& type);
};


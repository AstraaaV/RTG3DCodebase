#pragma once
#include <string>
class Light;

//ditto for the other factories but now for lights!
class LightFactory
{
public:

	static Light* makeNewLight(std::string _type);
};

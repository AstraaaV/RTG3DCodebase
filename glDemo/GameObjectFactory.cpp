#include "GameObjectFactory.h"
#include "GameObject.h"
#include "ExampleGO.h"
#include "Terrain.h"
#include <assert.h>

using std::string;

GameObject* GameObjectFactory::makeNewGO(string _type)
{
	printf("GAME OBJECT TYPE: %s \n", _type.c_str());
	if (_type == "GAME_OBJECT")
	{
		return new GameObject();
	}
	else if (_type == "EXAMPLE")
	{
		return new ExampleGO();
	}
	else if (_type == "TERRAIN")
	{
		return new Terrain();
	}
	else
	{
		printf("UNKNOWN GAME OBJECT TYPE: %s \n", _type.c_str());
		assert(0);
		return nullptr;
	}
}
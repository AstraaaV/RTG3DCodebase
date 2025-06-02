#include "GameObjectFactory.h"
#include "GameObject.h"
#include "Beast.h"
#include "Background.h"
#include "ExampleGO.h"
#include <iostream>
#include <assert.h>

using std::string;
using std::cout;
using std::endl;

GameObject* GameObjectFactory::makeNewGO(const string& type)
{
	cout << "[GAMEOBJECTFACTORY} Requested GameObject Type: " << type << endl;

	if (type == "GAME_OBJECT")
	{
		return new GameObject();
	}
	else if (type == "EXAMPLE")
	{
		return new ExampleGO();
	}
	else if (type == "BACKGROUND")
	{
		return new Background();
	}
	else if (type == "BEAST")
	{
		return new Beast();
	}
	else
	{
		cout << "[GAMEOBJECTFACTORY] Error. Unknown GameObject Type: " << type << endl;
		assert(false && "Unknown GameObject type passed to GameObjectFactory.");
		return nullptr;
	}
}

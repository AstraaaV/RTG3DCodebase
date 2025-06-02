#include "ModelFactory.h"
#include "AIModel.h"
#include "Cube.h"
#include <iostream>
#include <assert.h>

using std::string;
using std::cout;
using std::endl;

Model* ModelFactory::makeNewModel(const string& type)
{
	cout << "[MODELFACTORY] Requested Model Type: " << type << endl;

	//There is no point in making one of the model base class 
	//as it doesn't do anything 
	if (type == "AI")
	{
		return new AIModel();
	}
	if (type == "CUBE")
	{
		return new Cube();
	}
	else
	{
		cout << "[MODELFACTORY] Error. Unknown Model Type: " << type << endl;
		assert(false && "Unknown Model type passed to ModelFactory.");
		return nullptr;
	}
}

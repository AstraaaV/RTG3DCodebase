#pragma once
#include <string>
class Model;

//models are the things owned by Game Objects that are rendered at their location
//base factory to create them given a TYPE
class ModelFactory
{
public:

	static Model* makeNewModel(std::string _type);
};

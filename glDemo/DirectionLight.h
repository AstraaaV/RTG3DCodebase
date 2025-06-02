#pragma once
#include "Light.h"

//a simple directional light
//essentially only has one difference a direction variable
class DirectionLight :
	public Light
{
public:
	DirectionLight();
	~DirectionLight();

	//load from manifest
	virtual void Load(ifstream& _file);

	//set render values
	virtual void SetRenderValues(unsigned int _prog);

	//TODO: We don't have our own tick
	// a nice feature would be a day / night cycle effect 

protected:
	vec3 m_direction;

};

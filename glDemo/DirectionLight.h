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

	// Tick this light
	virtual void Tick(float _dt, GLFWwindow* window);
	//TODO: We don't have our own tick
	// a nice feature would be a day / night cycle effect 

protected:
	vec3 m_direction;

	// Day and Night cycle
	float m_timeOfDay = 0.0f;
	float m_speed = 5.0f; // how fast the cycle goes

};


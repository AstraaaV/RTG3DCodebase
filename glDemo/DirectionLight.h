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
	void Load(ifstream& _file) override;

	//set render values
	void SetRenderValues(unsigned int _prog) override;

	// Tick this light
	void Tick(float _dt, GLFWwindow* window) override;
protected:
	vec3 m_direction;

	// Day and Night cycle
	float m_timeOfDay = 0.0f;
	float m_speed = 5.0f; // how fast the cycle goes

};


#pragma once
#include "Light.h"

// Point light class with position and attenuation
class PointLight : public Light
{
public:
	PointLight();
	~PointLight();

	// Load pointlight from file
	virtual void Load(ifstream& _file);

	// Uploads all uniforms to shader program
	virtual void SetRenderValues(unsigned int _prog);

	// Getters
	vec3 GetPosition() const { return m_position; }
	float GetConstant() const { return m_constant; }
	float GetLinear() const { return m_linear; }
	float GetQuadratic() const { return m_quadratic; }

protected:
	vec3 m_position;

	// Attenuation values
	float m_constant;
	float m_linear;
	float m_quadratic;
};
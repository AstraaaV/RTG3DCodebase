#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();

	virtual void Load(ifstream& _file);

	virtual void SetRenderValues(unsigned int _prog);
	//virtual void Tick(float _dt);

	vec3 GetPosition() const { return m_position; }
	float GetConstant() const { return m_constant; }
	float GetLinear() const { return m_linear; }
	float GetQuadratic() const { return m_quadratic; }

protected:
	vec3 m_position;

	float m_constant;
	float m_linear;
	float m_quadratic;
};
#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();

	void Load(std::ifstream& _file) override;
	void SetRenderValues(unsigned int _prog) override;

	float m_constant;
	float m_linear;
	float m_quadratic;
};


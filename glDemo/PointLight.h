#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	~PointLight() override;

	void Load(std::ifstream& _file) override;
	void SetRenderValues(unsigned int _prog) override;

	float m_constant = 1.0f;
	float m_linear = 0.09f;
	float m_quadratic = 0.032f;
};


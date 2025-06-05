#include "PointLight.h"
#include "core.h"
#include "helper.h"
#include "stringHelp.h"

PointLight::PointLight()
{
	m_type = "POINT";
	m_constant = 1.0f;
	m_linear = 0.09f;
	m_quadratic = 0.032f;
}

PointLight::~PointLight()
{
}

void PointLight::Load(std::ifstream& _file)
{
	Light::Load(_file);

	StringHelp::Float(_file, "CONST", m_constant);
	StringHelp::Float(_file, "LIN", m_linear);
	StringHelp::Float(_file, "QUAD", m_quadratic);
}

void PointLight::SetRenderValues(unsigned int _prog)
{
	Light::SetRenderValues(_prog);
	GLint loc;

	string constString = m_name + "Constant";
	string linString = m_name + "Linear";
	string quadString = m_name + "Quadratic";

	if(Helper::SetUniformLocation(_prog, constString.c_str(), &loc))
	glUniform1f(loc, m_constant);

	if (Helper::SetUniformLocation(_prog, linString.c_str(), &loc))
	glUniform1f(loc, m_linear);

	if (Helper::SetUniformLocation(_prog, quadString.c_str(), &loc))
	glUniform1f(loc, m_quadratic);
}

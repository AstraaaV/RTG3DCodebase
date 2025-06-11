#include "core.h"
#include "PointLight.h"
#include "helper.h"
#include "stringHelp.h"

using namespace std;

PointLight::PointLight()
{
	m_type = "POINT";

	m_position = vec3(0.0f);
	m_col = vec3(1.0f);
	m_amb = vec3(0.05f);
	m_constant = 1.0f;
	m_linear = 0.09f;
	m_quadratic = 0.032f;
}

PointLight::~PointLight()
{
}

void PointLight::Load(ifstream& _file)
{
	Light::Load(_file);
	StringHelp::Float3(_file, "POS", m_position.x, m_position.y, m_position.z);
}

void PointLight::SetRenderValues(unsigned int _prog)
{
	Light::SetRenderValues(_prog);

	GLint loc;
	std::string prefix = m_name;

	if (Helper::SetUniformLocation(_prog, (prefix + "Pos").c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(m_position));

	if (Helper::SetUniformLocation(_prog, (prefix + "Constant").c_str(), &loc))
		glUniform1f(loc, m_constant);

	if (Helper::SetUniformLocation(_prog, (prefix + "Linear").c_str(), &loc))
		glUniform1f(loc, m_linear);

	if (Helper::SetUniformLocation(_prog, (prefix + "Quadratic").c_str(), &loc))
		glUniform1f(loc, m_quadratic);
}

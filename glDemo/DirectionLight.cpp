#include "core.h"
#include "DirectionLight.h"
#include "helper.h"
#include "stringHelp.h"

DirectionLight::DirectionLight()
{
	m_type = "DIRECTIONAL";
	m_pos = vec3(0.0, 0.0, 0.0);
	m_direction = vec3(0.0, 1.0, 0.0);
}

DirectionLight::~DirectionLight()
{
}

void DirectionLight::Load(ifstream& _file)
{
	Light::Load(_file);
	StringHelp::Float3(_file, "DIRECTIONAL", m_direction.x, m_direction.y, m_direction.z);
}

void DirectionLight::SetRenderValues(unsigned int _prog)
{
	//still need to tell the shader about the basic light data
	Light::SetRenderValues(_prog);

	GLint loc;
	string dirString = m_name + "SUN";//only thing I add is a direction

	if (Helper::SetUniformLocation(_prog, dirString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(m_direction));
}

void DirectionLight::Tick(float _dt, GLFWwindow* window)
{
	m_timeOfDay += _dt * m_speed;

	// Loop time
	if (m_timeOfDay > 360.0f)
		m_timeOfDay -= 360.0f;

	// Rotate light on XZ plane
	float radians = glm::radians(m_timeOfDay);
	m_direction = glm::normalize(vec3(cos(radians), sin(radians), 0.0f));

	// Dim light when its below "horizon"
	if (m_direction.y > 0.0f)
	{
		m_col = glm::vec3(1.0f, 1.0f, 0.9f);
		m_amb = glm::vec3(0.3f);
	}
	else
	{
		m_col = glm::vec3(0.1f, 0.1f, 0.2f);
		m_amb = glm::vec3(0.05f);
	}
}

#include "core.h"
#include "Light.h"

#include "helper.h"
#include "stringHelp.h"
#include <GLFW/glfw3.h>

Light::Light()
{
	m_type = "LIGHT";
	m_pos.x = 0.0f;
	m_pos.y = 0.0f;
	m_pos.z = 0.0f;
}

void Light::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "COL", m_col.x, m_col.y, m_col.z);
	StringHelp::Float3(_file, "AMB", m_amb.x, m_amb.y, m_amb.z);

}

/////////////////////////////////////////////////////////////////////////////////////
// Update() - 
/////////////////////////////////////////////////////////////////////////////////////
void Light::Tick(float _dt, GLFWwindow* window)
{
	static float timeOfDay = 0.0f;
	timeOfDay += _dt;

	float dayCycleSpeed = 0.1f;
	float lightFactor = (sin(timeOfDay * dayCycleSpeed) + 1.0f) / 2.0f;

	glm::vec3 daySky = glm::vec3(1.0f, 1.0f, 0.8f);
	glm::vec3 nightSky = glm::vec3(0.1f, 0.1f, 0.2f);

	m_col = glm::mix(nightSky, daySky, lightFactor);
	m_amb = glm::vec3(0.2f) * lightFactor;
}

//send values to the shaders to allow the use of this light
// <m_name>Pos <m_name>Col <m_name>Amb
void Light::SetRenderValues(unsigned int _prog)
{
	GLint loc;
	string posString = m_name + "Pos";
	string colString = m_name + "Col";
	string ambString = m_name + "Amb";

	if (Helper::SetUniformLocation(_prog, posString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetPos()));

	if (Helper::SetUniformLocation(_prog, colString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetCol()));

	if (Helper::SetUniformLocation(_prog, ambString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetAmb()));
}

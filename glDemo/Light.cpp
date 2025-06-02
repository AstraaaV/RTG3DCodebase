#include "core.h"
#include "Light.h"
#include "core.h"

#include "helper.h"
#include "stringHelp.h"
#include <GLFW/glfw3.h>
#include <iostream>

Light::Light()
{
	m_type = "LIGHT", m_name = "Light";
	
	m_pos = glm::vec3(0.0f);
	m_dir = glm::vec3(0.0f, -1.0f, 0.0f);
	m_col = glm::vec3(1.0f);
	m_amb = glm::vec3(0.2f);
	m_specular = glm::vec3(1.0f);
	m_diffuse = glm::vec3(1.0f);
	m_intensity = 1.0f;
}

Light::~Light()
{
}

void Light::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "DIR", m_dir.x, m_dir.y, m_dir.z);
	StringHelp::Float3(_file, "COL", m_col.x, m_col.y, m_col.z);
	StringHelp::Float3(_file, "AMB", m_amb.x, m_amb.y, m_amb.z);
	StringHelp::Float(_file, "INTENSITY", m_intensity);

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

void Light::SetRenderValues(GLuint _shaderProg)
{
	GLint loc;

	std::string base = m_name + ".";
	Helper::SetUniformLocation(_shaderProg, (base + "position").c_str(), &loc);
	glUniform3fv(loc, 1, glm::value_ptr(m_pos));

	Helper::SetUniformLocation(_shaderProg, (base + "direction").c_str(), &loc);
	glUniform3fv(loc, 1, glm::value_ptr(m_dir));

	Helper::SetUniformLocation(_shaderProg, (base + "colour").c_str(), &loc);
	glUniform3fv(loc, 1, glm::value_ptr(m_col));

	Helper::SetUniformLocation(_shaderProg, (base + "ambient").c_str(), &loc);
	glUniform3fv(loc, 1, glm::value_ptr(m_amb));

	Helper::SetUniformLocation(_shaderProg, (base + "intensity").c_str(), &loc);
	glUniform1f(loc, m_intensity);
}

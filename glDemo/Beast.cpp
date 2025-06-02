#include "Beast.h"
#include "shader_setup.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <helper.h>
#include <iostream>

Beast::Beast()
{
	m_shaderID = 0;
	m_rotation = 0.0f;
	m_idleTime = 0.0f;
	m_moveToEnd = true;
	SetName("Beast");

	m_mesh = new AIMesh("Assets\\beast\\beast.obj");
	if (m_mesh)
	{
		m_mesh->addTexture("Assets\\beast\\beast_texture.bmp", FIF_BMP);
	}
	else
	{
		std::cout << "Failed to load Beast mesh." << endl;
	}

	m_patrolStart = glm::vec3(11.0f, 0.0f, 11.0f);
	m_patrolEnd = m_patrolStart + glm::vec3(5.0f, 0.0f, 0.0f);
	m_position = m_patrolStart;
}

Beast::~Beast()
{
	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
}

void Beast::Tick(float dt, GLFWwindow* window)
{
	m_idleTime += dt;

	float idleOffset = sin(m_idleTime * 2.0f) * 0.2f;
	m_position.y = 0.5f + idleOffset;

	// Patrol
	glm::vec3 target = m_moveToEnd ? m_patrolEnd : m_patrolStart;
	glm::vec3 direction = glm::normalize(target - m_position);
	float distance = glm::length(target - m_position);

	if (distance > 0.1f)
	{
		m_position += direction * m_patrolSpeed * dt;
	}
	else
	{
		m_moveToEnd = !m_moveToEnd;
	}
}

void Beast::Render()
{
	if (!m_mesh || m_shaderID == 0) return;

	glUseProgram(m_shaderID);

	glm::mat4 model = glm::translate(glm::mat4(1), m_position) *
		glm::rotate(glm::mat4(1), glm::radians(m_rotation), glm::vec3(0, 1, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(0.7f));

	GLint loc;
	Helper::SetUniformLocation(m_shaderID, "modelMatrix", &loc);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);

	Helper::SetUniformLocation(m_shaderID, "texture", &loc);
	glUniform1i(loc, 0);

	m_mesh->setupTextures();
	m_mesh->render();
}

void Beast::Init(AIMesh* mesh, glm::vec3 start, glm::vec3 end)
{
	if(mesh)
		m_mesh = mesh;

	m_patrolStart = start;
	m_patrolEnd = end;
	m_position = start;
}

#include "Beast.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <helper.h>

Beast::Beast()
{
	m_shaderID = 0;
	m_position = glm::vec3(11.0f, 0.0f, 11.0f);
	m_rotation = 0.0f;

	m_mesh = new AIMesh("Assets\\beast\\beast.obj");
	if (m_mesh)
	{
		m_mesh->addTexture("Assets\\beast\\beast_texture.bmp", FIF_BMP);
	}

	SetName("Beast");
}

Beast::~Beast()
{
	delete m_mesh;
}

void Beast::Tick(float dt, GLFWwindow* window)
{
	m_rotation += 10.0f * dt;
}

void Beast::Render()
{
	if (!m_mesh) return;

	glm::mat4 model = glm::translate(glm::mat4(1), m_position) *
		glm::eulerAngleY(glm::radians(m_rotation));

	GLint loc;
	Helper::SetUniformLocation(m_shaderID, "modelMatrix", &loc);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);

	m_mesh->setupTextures();
	m_mesh->render();
}

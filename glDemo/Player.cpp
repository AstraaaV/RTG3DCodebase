#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <helper.h>
#include <GLFW/glfw3.h>
#include <Cube.h>
#include <iostream>

Player::Player()
{
	SetName("Player");
	m_position = glm::vec3(5.0f, 0.0f, 5.0f);
	m_cube = new Cube();
	m_speed = 5.0f;
}

Player::~Player() 
{
	if (m_cube)
	{
		delete m_cube;
		m_cube = nullptr;
	}
}

void Player::Tick(float dt, GLFWwindow* window)
{
	glm::vec3 move(0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) move.z -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) move.z += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) move.x -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) move.x += 1.0f;

	if (glm::length(move) > 0.0f)
		m_position += glm::normalize(move) * m_speed * dt;
}

void Player::Render()
{
	if (!m_shaderID)
	{
		cout << "[PLAYER] Shader not set.\n";
		return;
	}

	// Model Matrix
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position) *
		glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
	
	GLint loc;
	Helper::SetUniformLocation(m_shaderID, "modelMatrix", &loc);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);

	if (m_cube) m_cube->render();
}

void Player::SetPosition(const glm::vec3& pos)
{
	m_position = pos;
}

glm::vec3 Player::GetPosition() const
{
	return m_position;
}

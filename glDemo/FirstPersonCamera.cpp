#include "FirstPersonCamera.h"

using namespace std;
using namespace glm;


FirstPersonCamera::FirstPersonCamera()
{
	m_position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_pitch = 0.0f;
	m_yaw = -90.0f;
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_speed = 2.5f;
	m_sensitivity = 0.1f;

	m_fov = 60.0f;
	m_near = 0.1f;
	m_far = 100.0f;
	updateCamVectors();
}

void FirstPersonCamera::Init(float width, float height, Scene* scene)
{
	m_aspect = width / height;
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

void FirstPersonCamera::Tick(float dt, GLFWwindow* window)
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);

	int w = 1, h = 1;
	glfwGetFramebufferSize(window, &w, &h);
	m_aspect = static_cast<float>(w) / static_cast<float>(h);
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

void FirstPersonCamera::processKeys(CameraMovement direction, float deltaTime)
{
	float velocity = m_speed * deltaTime;

	switch (direction)
	{
	case CameraMovement::FORWARD:
		m_position += m_front * velocity;
		break;
	case CameraMovement::BACKWARD:
		m_position -= m_front * velocity;
		break;
	case CameraMovement::LEFT:
		m_position -= m_right * velocity;
		break;
	case CameraMovement::RIGHT:
		m_position += m_right * velocity;
		break;
	case CameraMovement::UP:
		m_position += m_up * velocity;
		break;
	case CameraMovement::DOWN:
		m_position -= m_up * velocity;
		break;
	}
}

void FirstPersonCamera::updateCamVectors()
{
	// Calculates front vector using pitch and yaw
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	// Recalculates the right and up vectors
	m_right = glm::normalize(glm::cross(m_front, m_up));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::vec3 FirstPersonCamera::getPosition() const
{
	return m_position;
}

void FirstPersonCamera::setPosition(const glm::vec3& _position)
{
	this->m_position = _position;
}

float FirstPersonCamera::getPitch() const
{
	return m_pitch;
}

void FirstPersonCamera::setPitch(float _pitch)
{
	this->m_pitch = _pitch;
	updateCamVectors();
}

float FirstPersonCamera::getYaw() const
{
	return m_yaw;
}

void FirstPersonCamera::setYaw(float _yaw)
{
	this->m_yaw = _yaw;
	updateCamVectors();
}

float FirstPersonCamera::getSensitivity() const
{
	return m_sensitivity;
}

glm::mat4 FirstPersonCamera::GetView() const
{
	glm::vec3 position = m_position;
	glm::vec3 target = m_position + m_direction;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 FirstPersonCamera::GetProj() const
{
	float size = 10.0f;
	return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

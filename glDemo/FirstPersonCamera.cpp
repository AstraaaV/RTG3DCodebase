#include "FirstPersonCamera.h"

using namespace std;
using namespace glm;

void FirstPersonCamera::calculateDerivedValues()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
	m_projectionMatrix = glm::perspective(glm::radians(m_fovY), m_aspect, m_nearClipPlane, m_farClipPlane);
}

void FirstPersonCamera::updateCamVectors()
{
	// Calculates front vector using pitch and yaw
	glm::vec3 front;
	front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	front.y = sin(glm::radians(m_pitch));
	front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_front = glm::normalize(front);

	// Recalculates the right and up vectors
	m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

FirstPersonCamera::FirstPersonCamera()
{
	m_position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_nearClipPlane = 0.1f;
	m_farClipPlane = 500.0f;

	m_pitch = 0.0f;
	m_yaw = -90.0f;

	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_speed = 2.5f;
	m_sensitivity = 0.1f;

	// calculate derived values
	updateCamVectors();
	calculateDerivedValues();
}

FirstPersonCamera::FirstPersonCamera(float _speed, float _sensitivity, float _pitch, float _yaw, float _fovY, float _aspect, float _nearPlane, float _farPlane)
{
	this->m_fovY = _fovY;
	this->m_aspect = _aspect;
	this->m_nearClipPlane = _nearPlane;
	this->m_farClipPlane = _farPlane;

	this->m_pitch = _pitch;
	this->m_yaw = _yaw;

	this->m_speed = _speed;
	this->m_sensitivity = _sensitivity;

	// calculate derived values
	updateCamVectors();
	calculateDerivedValues();
}

glm::vec3 FirstPersonCamera::getPosition() const
{
	return m_position;
}

void FirstPersonCamera::setPosition(const glm::vec3& _position)
{
	this->m_position = _position;
	calculateDerivedValues();
}

float FirstPersonCamera::getPitch() const
{
	return m_pitch;
}

void FirstPersonCamera::setPitch(float _pitch)
{
	this->m_pitch = _pitch;
	calculateDerivedValues();
}

float FirstPersonCamera::getYaw() const
{
	return m_yaw;
}

void FirstPersonCamera::setYaw(float _yaw)
{
	this->m_yaw = _yaw;
	calculateDerivedValues();
}

float FirstPersonCamera::getFovY()
{
	return m_fovY;
}

void FirstPersonCamera::setFovY(float _fovY)
{
	this->m_fovY = _fovY;
	calculateDerivedValues();
}

float FirstPersonCamera::getAspect()
{
	return m_aspect;
}

void FirstPersonCamera::setAspect(float _aspect)
{
	this->m_aspect = _aspect;
	calculateDerivedValues();
}

float FirstPersonCamera::getNearPlaneDistance()
{
	return m_nearClipPlane;
}

void FirstPersonCamera::setNearPlaneDistance(float _nearPlaneDistance)
{
	this->m_nearClipPlane = _nearPlaneDistance;
	calculateDerivedValues();
}

float FirstPersonCamera::getFarPlaneDistance()
{
	return m_farClipPlane;
}

void FirstPersonCamera::setFarPlaneDistance(float _farPlaneDistance)
{
	this->m_farClipPlane = _farPlaneDistance;
	calculateDerivedValues();
}

void FirstPersonCamera::processKeys(CameraMovement direction, float deltaTime)
{
	float velocity = m_speed * deltaTime;

	if (direction == CameraMovement::FORWARD)
		m_position += m_front * velocity;
	if (direction == CameraMovement::BACKWARD)
		m_position -= m_front * velocity;
	if (direction == CameraMovement::LEFT)
		m_position -= m_right * velocity;
	if (direction == CameraMovement::RIGHT)
		m_position += m_right * velocity;
	if (direction == CameraMovement::UP)
		m_position += m_up * velocity;
	if (direction == CameraMovement::DOWN)
		m_position -= m_up * velocity;

	calculateDerivedValues();
}

glm::mat4 FirstPersonCamera::viewTransform()
{
	return m_viewMatrix;
}

glm::mat4 FirstPersonCamera::projectionTransform()
{
	return m_projectionMatrix;
}

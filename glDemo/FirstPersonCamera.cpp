#include "FirstPersonCamera.h"
#include <iostream>

FirstPersonCamera::FirstPersonCamera()
{
	m_type = "FPC";

	m_pos = vec3(0.0f, 0.0f, 3.0f);
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_speed = 0.1;
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_yaw = -90.0f;
	m_pitch = 0.0f;

	m_fov = 45.0f;
	m_near = 0.1f;
	m_far = 100.0f;
	m_aspect = 1.0f;

	m_worldUp = vec3(0.0f, 1.0f, 0.0f);

	updateCamVectors();
}

void FirstPersonCamera::Init(float _w, float _h, Scene* scene)
{
	m_aspect = _w / _h;
	updateCamVectors();
}

void FirstPersonCamera::Tick(float _dt)
{
	updateCamVectors();
}

void FirstPersonCamera::MoveForward(float amount)
{
	m_pos += m_front * amount;
}

void FirstPersonCamera::MoveRight(float amount)
{
	m_pos += m_right * amount;
}

void FirstPersonCamera::MoveUp(float amount)
{
	m_pos += m_worldUp * amount;
}

void FirstPersonCamera::Rotate(float yawDt, float pitchDt)
{
	m_yaw += yawDt;
	m_pitch += pitchDt;

	if (m_pitch > 89.0f) m_pitch = 89.0f;
	if (m_pitch < -89.0f) m_pitch = -89.0f;

	updateCamVectors();
}

void FirstPersonCamera::ProcessMouse(float deltaX, float deltaY)
{
	float sensitivity = 0.1f;
	deltaX *= sensitivity;
	deltaY *= sensitivity;

	m_yaw += deltaX;
	m_pitch += deltaY;

	if (m_pitch > 89.0f) m_pitch = 89.0f;
	if (m_pitch < -89.0f) m_pitch = -89.0f;

	updateCamVectors();
}

void FirstPersonCamera::HandleKey(int key, int action)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if (key == GLFW_KEY_W)
		{
			m_pos += m_forward * m_speed;
		}
		else if (key == GLFW_KEY_S)
		{
			m_pos -= m_forward * m_speed;
		}
		else if (key == GLFW_KEY_A)
		{
			m_pos -= glm::normalize(glm::cross(m_forward, m_up)) * m_speed;
		}
		else if (key == GLFW_KEY_D)
		{
			m_pos += glm::normalize(glm::cross(m_forward, m_up)) * m_speed;
		}
	}
}

void FirstPersonCamera::updateCamVectors()
{
	vec3 front;
	front.x = cos(radians(m_yaw)) * cos(radians(m_pitch));
	front.y = sin(radians(m_pitch));
	front.z = sin(radians(m_yaw)) * cos(radians(m_pitch));
	m_front = normalize(front);

	m_right = normalize(cross(m_front, m_worldUp));
	m_up = normalize(cross(m_right, m_front));

	m_viewMatrix = lookAt(m_pos, m_pos + m_front, m_up);
	m_projectionMatrix = perspective(radians(m_fov), m_aspect, m_near, m_far);
}

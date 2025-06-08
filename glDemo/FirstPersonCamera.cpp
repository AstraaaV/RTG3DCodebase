#include "FirstPersonCamera.h"
#include <iostream>

FirstPersonCamera::FirstPersonCamera()
{
	m_type = "FPC";

	m_pos = vec3(0.0f, 0.0f, 3.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_worldUp = m_up;

	m_yaw = -90.0f;
	m_pitch = 0.0f;

	m_speed = 10.0f;
	m_fov = 45.0f;
	m_near = 0.1f;
	m_far = 100.0f;
	m_aspect = 1.0f;

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

	m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

	updateCamVectors();
}

void FirstPersonCamera::ProcessMouse(float deltaX, float deltaY)
{
	const float sensitivity = 0.15f;
	m_yaw += deltaX * sensitivity;
	m_pitch -= deltaY * sensitivity;

	m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

	updateCamVectors();
}

void FirstPersonCamera::HandleKey(int key, int action, float deltaTime)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		const float velocity = m_speed * deltaTime;

		switch (key)
		{
		case GLFW_KEY_W:
			MoveForward(velocity);
			break;
		case GLFW_KEY_S:
			MoveForward(-velocity);
			break;
		case GLFW_KEY_A:
			MoveRight(-velocity);
			break;
		case GLFW_KEY_D:
			MoveRight(velocity);
			break;
		case GLFW_KEY_UP:
			MoveUp(velocity);
			break;
		case GLFW_KEY_DOWN:
			MoveUp(-velocity);
			break;
		default:
			break;
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

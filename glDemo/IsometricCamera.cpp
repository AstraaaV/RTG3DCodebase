#include "IsometricCamera.h"

IsometricCamera::IsometricCamera()
{
	m_type = "ISO";
	m_pos = glm::vec3(10.0f, 10.0f, 10.0f);
	m_lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_fov = 45.0f;
	m_near = 0.1f;
	m_far = 100.0f;
	m_aspect = 1.0f;

	updateCam();
}

void IsometricCamera::Init(float width, float height, Scene* scene)
{
	m_aspect = width / height;
	updateCam();
}

void IsometricCamera::Tick(float dt)
{
	updateCam();
}

void IsometricCamera::Pan(const glm::vec3& direction)
{
	m_pos += direction;
	m_lookAt += direction;
	updateCam();
}

void IsometricCamera::Zoom(float amount)
{
	float orthoSize = 10.0f;
	orthoSize -= amount;

	orthoSize = glm::clamp(orthoSize, 2.0f, 50.0f);

	float halfW = orthoSize * m_aspect * 0.5f;
	float halfH = orthoSize * 0.5f;

	m_projectionMatrix = glm::ortho(-halfW, halfW, -halfH, halfH, m_near, m_far);
}

void IsometricCamera::updateCam()
{
	m_forward = glm::normalize(m_lookAt - m_pos);
	m_up = glm::normalize(m_worldUp);

	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, m_worldUp);
	
	float orthoSize = 10.0f;
	float halfW = orthoSize * m_aspect * 0.5f;
	float halfH = orthoSize * 0.5f;
	
	m_projectionMatrix = glm::ortho(-halfW, halfW, -halfH, halfH, m_near, m_far);
}

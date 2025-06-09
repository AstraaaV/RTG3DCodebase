#include "IsometricCamera.h"

IsometricCamera::IsometricCamera()
{
	m_type = "ISO";
	m_zoom = 1.0f;
	
	m_focus = glm::vec3(0.0f, 0.0f, 0.0f);
	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
	m_focus += direction;
	updateCam();
}

void IsometricCamera::Zoom(float amount)
{
	m_zoom -= amount;
	if (m_zoom < 0.1f) m_zoom = 0.1f;
	if (m_zoom > 10.0f) m_zoom = 10.0f;
	updateCam();
}

void IsometricCamera::SetZoom(float z)
{
	m_zoom = glm::clamp(z, 0.1f, 10.0f);
	updateCam();
}

void IsometricCamera::SetFocus(const glm::vec3& f)
{
	m_focus = f;
	updateCam();
}

void IsometricCamera::updateCam()
{
	const float angle = glm::radians(35.264f);
	const float rotAngle = glm::radians(45.0f);

	float dist = 10.0f * m_zoom;

	float x = dist * cos(angle) * cos(rotAngle);
	float y = dist * sin(angle);
	float z = dist * cos(angle) * sin(rotAngle);

	m_pos = m_focus + glm::vec3(x, y, z);

	m_viewMatrix = glm::lookAt(m_pos, m_focus, m_worldUp);

	float orthoSize = 10.0f * m_zoom;
	float halfW = orthoSize * m_aspect * 0.5f;
	float halfH = orthoSize * 0.5f;

	m_projectionMatrix = glm::ortho(-halfW, halfW, -halfH, halfH, m_near, m_far);

	m_forward = glm::normalize(m_focus - m_pos);
	m_up = m_worldUp;
}

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
	m_focus = glm::vec3(12.0f, 0.0f, 12.0f);

	float dist = 40.0f * m_zoom;

	m_pos = m_focus + glm::vec3(0.0f, dist, 0.0f);

	glm::vec3 upVec = glm::vec3(1.0f, 0.0f, 0.0f);

	m_viewMatrix = glm::lookAt(m_pos, m_focus, upVec);

	float orthoX = 30.0f * m_aspect * m_zoom;
	float orthoZ = 30.0f * m_zoom;

	float halfW = orthoX * 0.5f;
	float halfH = orthoZ * 0.5f;

	m_projectionMatrix = glm::ortho(-halfW, halfW, -halfH, halfH, m_near, m_far);

	m_forward = glm::normalize(m_focus - m_pos);
	m_up = upVec;
}

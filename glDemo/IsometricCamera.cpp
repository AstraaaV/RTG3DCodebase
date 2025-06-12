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
	m_zoom = glm::clamp(m_zoom, 0.1f, 10.0f);
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
	// Iso angles
	float pitch = glm::radians(35.264f);
	float yaw = glm::radians(45.0f);

	// Distance from focus point (for zooming)
	float dist = 40.0f * m_zoom;

	m_pos = m_focus + glm::vec3
	(dist * cos(pitch) * cos(yaw),
		dist * sin(pitch),
		dist * cos(pitch) * sin(yaw));

	m_viewMatrix = glm::lookAt(m_pos, m_focus, m_worldUp);

	// Orthographic projection
	float orthoW = 30.0f * m_aspect * m_zoom;
	float orthoH = 30.0f * m_zoom;

	m_projectionMatrix = glm::ortho
	(-orthoW * 0.5f, orthoW * 0.5f,
		-orthoH * 0.5f, orthoH * 0.5f,
		m_near, m_far);

	m_forward = glm::normalize(m_focus - m_pos);
	m_up = m_worldUp;
}

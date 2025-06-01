#include "IsometricCamera.h"
#include "Scene.h"

using namespace std;
using namespace glm;

IsometricCamera::IsometricCamera()
{
	m_focus = vec3(5.0f, 0.0f, 5.0f);
	m_zoom = 15.0f;
	m_angle = glm::radians(45.0f);

	m_fov = 45.0f;
	m_near = 0.1f;
	m_far = 100.0f;
	m_type = "Isometric";
	m_name = "IsometricCam";
}

void IsometricCamera::Init(float screenW, float screenH, Scene* scene)
{
	Camera::Init(screenW, screenH, scene);
	Tick(0.0f, nullptr);
}

void IsometricCamera::Tick(float dt, GLFWwindow* window)
{
	glm::vec3 offset
	(
		m_zoom * cos(m_angle),
		m_zoom * sin(m_angle),
		m_zoom * cos(m_angle)
	);

	m_pos = m_focus + offset;
	m_lookAt = m_focus;

	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0, 1, 0));

	if (window)
	{
		int w = 1, h = 1;
		glfwGetFramebufferSize(window, &w, &h);
		if (w <= 0) w = 1;
		if (h <= 0) h = 1;

		SetAspect(static_cast<float>(w) / static_cast<float>(h));
	}
}

void IsometricCamera::SetFocusPoint(const glm::vec3& focus)
{
	m_focus = focus;
}

void IsometricCamera::SetZoom(float zoomLevel)
{
	m_zoom = zoomLevel;
}

glm::mat4 IsometricCamera::GetView() const
{
	return m_viewMatrix;
}

glm::mat4 IsometricCamera::GetProj() const
{
	return glm::ortho(-m_viewSize * m_aspect, m_viewSize * m_aspect, -m_viewSize, m_viewSize, m_near, m_far);
}


#include "ArcballCamera.h"

using namespace std;
using namespace glm;

//
// Private API
//

// initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
ArcballCamera::ArcballCamera()
{
	m_target = glm::vec3(0.0f, 0.0f, 0.0f);
	m_distance = 10.0f;
	m_yaw = -90.0f;
	m_pitch = 20.0f;
	m_rotateSpeed = 0.1f;

	m_fov = 45.0f;
	m_near = 0.1f;
	m_far = 100.0f;
	m_type = "Arcball";
	m_name = "ArcballCam";
}

void ArcballCamera::Init(float screenW, float screenH, Scene* scene)
{
	Camera::Init(screenW, screenH, scene);
	UpdateCameraVectors();
}

void ArcballCamera::Tick(float dt, GLFWwindow* window)
{
	if (!window) return;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		double xpos, ypos;
		static double lastX = 0.0, lastY = 0.0;
		glfwGetCursorPos(window, &xpos, &ypos);

		double dx = xpos - lastX;
		double dy = ypos - lastY;

		m_yaw += static_cast<float>(dx) * m_rotateSpeed;
		m_pitch -= static_cast<float>(dy) * m_rotateSpeed;

		m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

		UpdateCameraVectors();

		lastX = xpos;
		lastY = ypos;
	}
	Camera::Tick(dt, window);
}

void ArcballCamera::SetTarget(glm::vec3 target)
{
	m_target = target;
	UpdateCameraVectors();
}

void ArcballCamera::SetDistance(float distance)
{
	m_distance = distance;
	UpdateCameraVectors();
}

void ArcballCamera::SetRotateSpeed(float speed)
{
	m_rotateSpeed = speed;
}

glm::mat4 ArcballCamera::GetView() const
{
	glm::vec3 position = m_pos;
	glm::vec3 target = m_pos + m_direction;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	return glm::lookAt(position, target, up);
}

glm::mat4 ArcballCamera::GetProj() const
{
	float size = 10.0f;
	return glm::ortho(-size * m_aspect, size * m_aspect, -size, size, m_near, m_far);
}

void ArcballCamera::Zoom(float offset)
{
	if (offset < 0.0f)
		scaleRadius(1.1f);
	else
		scaleRadius(0.9f);
}

void ArcballCamera::scaleRadius(float factor)
{
	m_distance *= factor;

	if (m_distance < 1.0f)
		m_distance = 1.0f;
	if (m_distance > 100.0f)
		m_distance = 100.0f;

	UpdateCameraVectors();
}

void ArcballCamera::UpdateCameraVectors()
{
	float yawRad = glm::radians(m_yaw);
	float pitchRad = glm::radians(m_pitch);

	glm::vec3 direction;
	direction.x = cos(pitchRad) * cos(yawRad);
	direction.y = sin(pitchRad);
	direction.z = cos(pitchRad) * sin(yawRad);

	m_pos = m_target - glm::normalize(direction) * m_distance;
	m_lookAt = m_target;

	m_viewMatrix = glm::lookAt(m_pos, m_target, glm::vec3(0, 1, 0));
}
#pragma endregion
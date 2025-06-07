#pragma once
#include "Camera.h"

class IsometricCamera : public Camera
{
public:
	IsometricCamera();
	void Init(float width, float height, Scene* scene) override;
	void Tick(float dt) override;

	void Pan(const glm::vec3& direction);
	void Zoom(float amount);

	glm::mat4 GetView() override { return m_viewMatrix; }
	glm::mat4 GetProj() override { return m_projectionMatrix; }

	void SetZoom(float z);
	void SetFocus(const glm::vec3& f);

private:
	void updateCam();

	glm::vec3 m_pos;
	glm::vec3 m_lookAt;
	glm::vec3 m_worldUp;

	glm::vec3 m_forward;
	glm::vec3 m_up;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionmatrix;

	glm::vec3 m_focus = glm::vec3(0);

	float m_fov;
	float m_near;
	float m_far;
	float m_aspect;
	float m_zoom = 1.0f;
};
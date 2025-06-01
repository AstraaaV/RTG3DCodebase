#pragma once

#include "Camera.h"
#include "core.h"

class IsometricCamera : public Camera
{
public:
	// Constructors
	IsometricCamera();
	~IsometricCamera() override = default;

	void Init(float screenW, float screenH, Scene* scene) override;
	void Tick(float dt, GLFWwindow* window) override;

	void SetFocusPoint(const glm::vec3& focus);
	void SetZoom(float zoomLevel);

	glm::mat4 GetView() const override;
	glm::mat4 GetProj() const override;

private:
	glm::vec3 m_focus = glm::vec3(0.0f);
	glm::vec3 m_direction = glm::vec3(0.0f, -1.0f, -1.0f);
	float m_zoom = 1.0f;
	float m_angle = 45.0f;
	float m_viewSize = 10.0f;

};


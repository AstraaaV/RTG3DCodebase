#pragma once
#include "Camera.h"

class IsometricCamera : public Camera
{
public:
	IsometricCamera();
	void Init(float width, float height, Scene* scene) override;
	void Tick(float dt) override;

	glm::mat4 GetView() override { return m_viewMatrix; }
	glm::mat4 GetProj() override { return m_projectionMatrix; }

private:
	void updateCam();
};
#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

class GameObject;

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera();
	virtual ~FirstPersonCamera() {}

	void Init(float _w, float _h, Scene* scene) override;
	void Tick(float _dt) override;

	void MoveForward(float amount);
	void MoveRight(float amount);
	void MoveUp(float amount);
	void Rotate(float yawDt, float pitchDt);

	void ProcessMouse(float deltaX, float deltaY);
	void HandleKey(int key, int action, float deltaTime);

	glm::mat4 GetView() override { return m_viewMatrix; }
	glm::mat4 GetProj() override { return m_projectionMatrix; }

	glm::vec3 GetForward() const { return m_front; }

	void SetTarget(GameObject* targ);
	void SetYaw(float yaw);

private:
	float m_yaw, m_pitch;
	
	glm::vec3 m_pos;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	
	float m_speed;
	
	glm::vec3 m_worldUp;

	GameObject* m_target = nullptr;

	void updateCamVectors();
};
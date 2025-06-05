#pragma once

#include "Camera.h"
#include "core.h"

enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera();

	void Init(float width, float height, Scene* scene) override;
	void Tick(float dt, GLFWwindow* window) override;

	void processKeys(CameraMovement direction, float deltaTime);

	glm::vec3 getPosition() const;
	void setPosition(const glm::vec3& _position);

	float getYaw() const;
	void setYaw(float _yaw);

	float getPitch() const;
	void setPitch(float _pitch);

	float getSensitivity() const;

	glm::mat4 GetView() const override;
	glm::mat4 GetProj() const override;
private:
	void updateCamVectors();

	glm::vec3 m_position, m_front, m_up, m_right;

	float m_pitch, m_yaw;

	float m_speed, m_sensitivity;

	//glm::vec3 m_direction; reserved for later
};


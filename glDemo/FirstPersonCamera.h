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
private:
	glm::vec3 m_position, m_front, m_up, m_right;

	float m_fovY, m_aspect, m_nearClipPlane, m_farClipPlane;

	float m_pitch, m_yaw;

	float m_speed, m_sensitivity;

	// view transform matrix for camera's current position and orientation - maps from world to eye coordinate space
	glm::mat4			m_viewMatrix;

	// projection transform matrix
	glm::mat4			m_projectionMatrix;

	void calculateDerivedValues();
	void updateCamVectors();

public:
	FirstPersonCamera();
	FirstPersonCamera(float _speed, float _sensitivity, float _pitch, float _yaw, float _fovY, float _aspect, float _nearPlane, float _farPlane);

	glm::vec3 getPosition() const;
	void setPosition(const glm::vec3& _position);

	float getPitch() const;
	void setPitch(float _pitch);

	float getYaw() const;
	void setYaw(float _yaw);

	float getFovY();
	void setFovY(float _fovY);

	float getAspect();
	void setAspect(float _aspect);

	float getNearPlaneDistance();
	void setNearPlaneDistance(float _nearPlaneDistance);

	float getFarPlaneDistance();
	void setFarPlaneDistance(float _farPlaneDistance);

	float getSensitivity() const { return m_sensitivity; }

	void processKeys(CameraMovement direction, float deltaTime);

	glm::mat4 viewTransform(); // return a const reference to the view transform matrix for the camera
	glm::mat4 projectionTransform(); // return a const reference the projection transform for the camera.  This is a pass-through method and calls projectionMatrix on the encapsulated ViewFrustum
};


#pragma once

#include "Camera.h"
#include "core.h"

// Model an arcball / pivot camera looking at the origin (0, 0, 0).  
// The camera by default looks down the negative z axis (using a right-handed coordinate system).  
// Therefore 'forwards' is along the -z axis.  The camera is actually right/left handed agnostic.  
// The encapsulated frustum however needs to know the differences for the projection matrix and frustum plane calculations

class ArcballCamera : public Camera {
public:

	// Constructors
	ArcballCamera(); // initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
	// create a camera with orientation <theta, phi> representing Euler angles specified in degrees and Euclidean distance 'init_radius' from the origin.  The frustum / viewplane projection coefficients are defined in init_fovy, specified in degrees spanning the entire vertical field of view angle, init_aspect (w/h ratio), init_nearPlane and init_farPlane.  If init_farPlane = 0.0 (as determined by equalf) then the resulting frustum represents an infinite perspective projection.  This is the default

	void Init(float screenW, float screenH, Scene* scene) override;
	void Tick(float dt, GLFWwindow* window) override;

	void SetTarget(glm::vec3 target);
	void SetDistance(float distance);
	void SetRotateSpeed(float speed);

	glm::mat4 GetView() const override;
	glm::mat4 GetProj() const override;

	void Zoom(float offset);
	void scaleRadius(float factor);

private:
	void UpdateCameraVectors();

	glm::vec3 m_target;
	float m_distance;
	float m_yaw, m_pitch;
	float m_rotateSpeed;
	glm::vec3 m_direction;
};

#pragma once

#include "Camera.h"
#include "core.h"

class IsometricCamera : public Camera
{
private:
	float				m_theta, m_phi; // spherical coordinates theta (rotation around the x axis) and phi (rotation around the y axis).  <theta, phi> are stored in degrees.  Zero degree rotation on <theta, phi> places the camera on the +z axis.  A positive phi represents counter-clockwise rotation around the y axis in a right-hand coordinate system.  A positive theta represents a counter-clockwise rotation around the x axis in a right-handed coordinate system
	float				m_radius; // radius of the camera's spherical coordinate model.  This lies in the interval [0, +inf]

	// Projection values
	float m_orthoSize; // Scale factor for zooming in & out
	float m_aspect; // Aspect ratio of view
	float m_nearClipPlane; // Near clipping plane
	float m_farClipPlane; // Far clipping plane

	// Derived values
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	void calculateDerivedValues(); // update position, orientation, view and projection matrices when camera rotation and radius are modified
public:
	// Constructors
	IsometricCamera();
	IsometricCamera(float _theta, float _phi, float _orthoSize, float _aspect, float _nearClipPlane, float _farClipPlane);

	// return the pivot rotation around the x axis (theta) in degrees
	float getTheta();

	// return the pivot rotation around the y axis (phi) in degrees
	float getPhi();

	float getOrthoSize();
	void setOrthoSize(float _orthoSize);

	float getAspect();
	void setAspect(float _aspect);

	float getNearPlaneDistance();
	void setNearPlaneDistance(float _nearPlaneDistance);

	float getFarPlaneDistance();
	void setFarPlaneDistance(float _farPlaneDistance);

	// Accessor methods for derived values
	glm::mat4 viewTransform(); // return a const reference to the view transform matrix for the camera
	glm::mat4 projectionTransform(); // return a const reference the projection transform for the camera.  This is a pass-through method and calls projectionMatrix on the encapsulated ViewFrustum
};


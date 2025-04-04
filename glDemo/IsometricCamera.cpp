#include "IsometricCamera.h"

using namespace std;
using namespace glm;

void IsometricCamera::calculateDerivedValues()
{
	const float theta_ = glm::radians<float>(m_theta);
	const float phi_ = glm::radians<float>(m_phi);

	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(-35.26f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// View matrix. combines rotations
	m_viewMatrix = rotationY * rotationX;

	// Projection matrix. orthographic for isometric
	m_projectionMatrix = glm::ortho(-m_orthoSize * m_aspect, m_orthoSize * m_aspect,
		-m_orthoSize, m_orthoSize, m_nearClipPlane, m_farClipPlane);
}

IsometricCamera::IsometricCamera()
{
	m_theta = 35.26f; // Top down view
	m_phi = 45.0f; // Side view

	m_orthoSize = 10.0f; // Default orthographic size
	m_aspect = 1.0f; // Aspect ratio
	m_nearClipPlane = 0.1f; // Near clipping plane
	m_farClipPlane = 1000.0f; // Far clipping plane

	calculateDerivedValues();
}

IsometricCamera::IsometricCamera(float _theta, float _phi, float _orthoSize, float _aspect, float _nearClipPlane, float _farClipPlane)
{
	this->m_theta = _theta;
	this->m_phi = _phi;

	this->m_orthoSize = _orthoSize;
	this->m_aspect = _aspect;
	this->m_nearClipPlane = _nearClipPlane;
	this->m_farClipPlane = _farClipPlane;

	calculateDerivedValues();
}

float IsometricCamera::getTheta()
{
	return m_theta;
}

float IsometricCamera::getPhi()
{
	return m_phi;
}

float IsometricCamera::getOrthoSize()
{
	return m_orthoSize;
}

void IsometricCamera::setOrthoSize(float _orthoSize)
{
	this->m_orthoSize = _orthoSize;
	calculateDerivedValues();
}

float IsometricCamera::getAspect()
{
	return m_aspect;
}

void IsometricCamera::setAspect(float _aspect)
{
	this->m_aspect = _aspect;
	calculateDerivedValues();
}

float IsometricCamera::getNearPlaneDistance()
{
	return m_nearClipPlane;
}

void IsometricCamera::setNearPlaneDistance(float _nearPlaneDistance)
{
	this->m_nearClipPlane = _nearPlaneDistance;
	calculateDerivedValues();
}

float IsometricCamera::getFarPlaneDistance()
{
	return m_farClipPlane;
}

void IsometricCamera::setFarPlaneDistance(float _farPlaneDistance)
{
	this->m_farClipPlane = _farPlaneDistance;
}

glm::mat4 IsometricCamera::viewTransform()
{
	return m_viewMatrix;
}

glm::mat4 IsometricCamera::projectionTransform()
{
	return m_projectionMatrix;
}

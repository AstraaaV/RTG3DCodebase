
#include "ArcballCamera.h"

using namespace std;
using namespace glm;

// ArcballCamera constructors

// initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
ArcballCamera::ArcballCamera() {

	m_theta = 0.0f;
	m_phi = 0.0f;
	m_radius = 15.0f;

	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 500.0f;

	//F = ViewFrustum(55.0f, 1.0f, 0.1f, 500.0f);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}


// create a camera with orientation <theta, phi> representing Euler angles specified in degrees and Euclidean distance 'init_radius' from the origin.  The frustum / viewplane projection coefficients are defined in init_fovy, specified in degrees spanning the entire vertical field of view angle, init_aspect (w/h ratio), init_nearPlane and init_farPlane.  If init_farPlane = 0.0 (as determined by equalf) then the resulting frustum represents an infinite perspective projection.  This is the default
ArcballCamera::ArcballCamera(float _theta, float _phi, float _radius, float _fovY, float _aspect, float _nearPlane, float _farPlane) {

	this->m_theta = _theta;
	this->m_phi = _phi;
	this->m_radius = std::max<float>(0.0f, _radius);

	this->m_fovY = _fovY;
	this->m_aspect = _aspect;
	this->m_nearPlane = _nearPlane;
	this->m_farPlane = _farPlane;

	//F = ViewFrustum(init_fovy, init_aspect, init_nearPlane, init_farPlane);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}

// update position, orientation and view matrices when camera rotation and radius is modified
void ArcballCamera::calculateDerivedValues() {

	float thetaRad = glm::radians(m_theta);
	float phiRad = glm::radians(m_phi);

	glm::vec3 direction;
	direction.x = m_radius * sinf(thetaRad) * sinf(phiRad);
	direction.y = m_radius * cosf(thetaRad);
	direction.z = m_radius * sinf(thetaRad) * cosf(phiRad);

	glm::vec3 position = m_target + direction;

	glm::vec3 up(0.0f, 1.0f, 0.0f);

	m_viewMatrix = glm::lookAt(position, m_target, up);

	// calculate position vector
	//cameraPos = glm::vec4(sinf(phi_) * cosf(-theta_) * radius, sinf(-theta_) * radius, cosf(phi_) * cosf(-theta_) * radius, 1.0f);

	// calculate orientation basis R
	//R = glm::eulerAngleY(phi_) * glm::eulerAngleX(theta_);

	// calculate view and projection transform matrices
	m_projectionMatrix = glm::perspective(glm::radians<float>(m_fovY), m_aspect, m_nearPlane, m_farPlane);
}


#pragma region Accessor methods for stored values

float ArcballCamera::getTheta()
{
	return m_theta;
}

float ArcballCamera::getPhi()
{
	return m_phi;
}

float ArcballCamera::getRadius()
{
	return 0.0f;
}

float ArcballCamera::getFovY()
{
	return 0.0f;
}

float ArcballCamera::getAspect()
{
	return 0.0f;
}

float ArcballCamera::getNearPlaneDistance()
{
	return 0.0f;
}

float ArcballCamera::getFarPlaneDistance()
{
	return 0.0f;
}

void ArcballCamera::rotateCamera(float dTheta, float dPhi)
{
	m_theta += dTheta;
	m_phi += dPhi;

	if (m_theta > 89.0f) m_theta = 89.0f;
	if (m_theta < -89.0f) m_theta = -89.0f;

	calculateDerivedValues();
}

void ArcballCamera::scaleRadius(float _s)
{
	if (_s > 0.0f)
	{
		m_radius *= _s;
		calculateDerivedValues();
	}
}

void ArcballCamera::incrementRadius(float _i)
{
	m_radius += _i;
	if (m_radius < 0.0f) m_radius = 0.0f;
	calculateDerivedValues();
}

void ArcballCamera::setFovY(float _fovY)
{
	m_fovY = _fovY;
	calculateDerivedValues();
}

void ArcballCamera::setAspect(float _aspect)
{
	m_aspect = _aspect;
	calculateDerivedValues();
}

void ArcballCamera::setNearPlaneDistance(float _nearPlaneDistance)
{
	m_nearPlane = _nearPlaneDistance;
	calculateDerivedValues();
}

void ArcballCamera::setFarPlaneDistance(float _farPlaneDistance)
{
	m_farPlane = _farPlaneDistance;
	calculateDerivedValues();
}

void ArcballCamera::AdjustYaw(float delta)
{
	m_phi += delta;
	calculateDerivedValues();
}

void ArcballCamera::AdjustPitch(float delta)
{
	m_theta += delta;
	if (m_theta > 89.0f) m_theta = 89.0f;
	if (m_theta < -89.0f) m_theta = -89.0f;
	calculateDerivedValues();
}

void ArcballCamera::Zoom(float delta)
{
	m_radius += delta;
	if (m_radius < 2.0f) m_radius = 2.0f;
	if (m_radius > 100.0f) m_radius = 100.0f;
	calculateDerivedValues();
}

void ArcballCamera::SetRadius(float r)
{
	m_radius = r;
	calculateDerivedValues();
}

#pragma endregion


#pragma region Accessor methods for derived values

// return the camera location in world coordinate space
//glm::vec4 ArcballCamera::getPosition() {
//
//	return cameraPos;
//}

// return a const reference to the camera's orientation matrix in world coordinate space
//glm::mat4 ArcballCamera::getOrientationBasis() {
//
//	return R;
//}

// return a const reference to the view transform matrix for the camera
glm::mat4 ArcballCamera::viewTransform() {

	return m_viewMatrix;
}

// return a const reference the projection transform for the camera
glm::mat4 ArcballCamera::projectionTransform() {

	return m_projectionMatrix;
}

#pragma endregion
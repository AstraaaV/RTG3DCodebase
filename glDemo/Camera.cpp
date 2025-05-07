#include "Camera.h"
#include "helper.h"
#include <fstream>
#include <iostream>
#include "stringHelp.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	m_type = "CAMERA";
}

/////////////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// Init() - 
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Init(float _screenWidth, float _screenHeight, Scene* _scene)
{
	setAspect(_screenWidth / _screenHeight);
}

/////////////////////////////////////////////////////////////////////////////////////
// Update() - 
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Tick(float _dt, GLFWwindow* window)
{
	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, vec3(0, 1, 0));

	// Ensure window is valid before using it
	if (!window) {
		std::cerr << "ERROR: GLFW window is null in Camera::Tick()" << std::endl;
		return;
	}

	// Update aspect ratio based on window size
	int width = 1, height = 1;
	glfwGetFramebufferSize(window, &width, &height);

	if (width <= 0) width = 1;
	if (height <= 0) height = 1;

	float asp = static_cast<float>(width) / static_cast<float>(height);
	setAspect(asp);
}

void Camera::setAspect(float asp)
{
	m_aspect = asp;
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), asp, m_near, m_far);
}

void Camera::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	StringHelp::Float(_file, "FOV", m_fov);
	StringHelp::Float(_file, "NEAR", m_near);
	StringHelp::Float(_file, "FAR", m_far);
}

//set the base render values for this camera in the shaders
void Camera::SetRenderValues(unsigned int _prog)
{
	GLint loc;

	//matrix for the view transform
	if (Helper::SetUniformLocation(_prog, "viewMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetView()));

	//matrix for the projection transform
	if (Helper::SetUniformLocation(_prog, "projMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetProj()));

	//the current camera is at this position
	if (Helper::SetUniformLocation(_prog, "camPos", &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetPos()));
}

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
	m_pos = glm::vec3(0.0f, 0.0f, 3.0f);
	m_lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fov = 45.0f;
	m_near = 0.1f;
	m_far = 100.0f;
	m_aspect = 1.0f;
	m_type = "BaseCam";

	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

/////////////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

//set the base render values for this camera in the shaders
void Camera::SetRenderValues(unsigned int shaderProgram)
{
	GLint loc;

	glm::mat4 viewMatrix = GetView();
	glm::mat4 projMatrix = GetProj();

	//matrix for the view transform
	if (Helper::SetUniformLocation(shaderProgram, "viewMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	//matrix for the projection transform
	if (Helper::SetUniformLocation(shaderProgram, "projMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projMatrix));
}

/////////////////////////////////////////////////////////////////////////////////////
// Init() - 
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Init(float _screenWidth, float _screenHeight, Scene* _scene)
{
	m_aspect = _screenWidth / _screenHeight;
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
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
	SetAspect(asp);
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

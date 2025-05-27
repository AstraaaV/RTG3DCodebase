#pragma once
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"
#include "core.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

using namespace glm;

class cTransform;
class Light;
class Scene;

//base class for a camera

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Init(float _w, float _h, Scene* _scene);
	//TODO: possibly pass keyboard / mouse stuff down here for player controls?
	virtual void Tick(float _dt, GLFWwindow* window);

	virtual glm::mat4 GetView() const = 0;
	virtual glm::mat4 GetProj() const = 0;

	string GetName() { return m_name; }
	void SetName(string _name) { m_name = _name; }
	string GetType() { return m_type; }

	glm::vec3 GetPos() { return m_pos; }
	void Move(glm::vec3 _d) { m_pos += _d; }

	//where am I looking at
	vec3 GetLookAt() { return m_lookAt; }
	void SetLookAt(vec3 _pos) { m_lookAt = _pos; }

	float GetFOV() { return m_fov; }
	float GetNear() { return m_near; }
	float GetFar() { return m_far; }
	void SetAspect(float asp) { m_aspect = asp; }

	//set up shader values for when using this camera
	virtual void SetRenderValues(unsigned int shaderProgram);

	//load camera info from the mainfest
	virtual void Load(ifstream& _file);

	virtual void Zoom(float offset) {}

protected:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

	glm::vec3 m_pos;
	glm::vec3 m_lookAt;

	float m_fov; //field of view
	float m_near;//near plane distance
	float m_far;//far plane distance
	float m_aspect = 1.0f;

	string m_name = "UnnamedCam";
	string m_type = "BaseCam";
};


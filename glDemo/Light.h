#pragma once
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

struct GLFWwindow;

using namespace glm;

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//base class for a light
class Light
{
public:
	Light();
	virtual ~Light();

	virtual void Tick(float _dt, GLFWwindow* window);
	
	//load from mainfest
	virtual void Load(ifstream& _file);
	virtual void SetRenderValues(unsigned int _prog);

	void SetPosition(const glm::vec3& pos) { m_pos = pos; }
	glm::vec3 GetPosition() const { return m_pos; }

	void SetColour(const glm::vec3& col) { m_col = col; }
	glm::vec3 GetColour() const { return m_col; }

	void SetAmbient(const glm::vec3& amb) { m_amb = amb; }
	glm::vec3 GetAmbient() const { return m_amb; }

	void SetName(const std::string& name) { m_name = name; }
	std::string GetName() const { return m_name; }

	void SetType(const std::string& type) { m_type = type; }
	std::string GetType() const { return m_type; }

protected:
	string m_name;
	string m_type;

	vec3 m_pos; // position of the light
	vec3 m_dir;
	vec3 m_col; // colour of the light
	vec3 m_amb; // ambient colour of the light
	float m_intensity;

};
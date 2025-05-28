#pragma once

#include "GameObject.h"
#include "AIMesh.h"
#include <glm/glm.hpp>

class Beast : public GameObject
{
public:
	Beast();
	~Beast();

	void Tick(float dt, GLFWwindow* window) override;
	void Render() override;
private:
	AIMesh* m_mesh = nullptr;
	glm::vec3 m_position;
	float m_rotation = 0.0f;

	GLuint m_shaderID;
};
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
	float m_idleTime = 0.0f;

	glm::vec3 m_patrolStart;
	glm::vec3 m_patrolEnd;
	float m_patrolSpeed = 1.5f;
	bool m_moveToEnd = true;
};
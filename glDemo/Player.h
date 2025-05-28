#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

class Cube;

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Tick(float dt, GLFWwindow* window) override;
	void Render() override;

	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition() const;
private:
	glm::vec3 m_position;
	float m_speed = 5.0f;
	GLuint m_shaderID;
	Cube* m_cube;
};
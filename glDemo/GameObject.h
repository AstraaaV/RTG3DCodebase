#pragma once
#include "core.h"
#include <string>

using namespace std;
class Scene;

using namespace glm;

//Base class of a GameObject
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init(Scene* scene);
	virtual void Tick(float deltaTime, GLFWwindow* window);
	virtual void Render();
	virtual void PreRender() {}
	virtual void Load(std::ifstream& file);
	virtual void SetShader(GLuint id) { m_shaderID = id; }

	void SetName(const std::string& name) { m_name = name; }
	std::string GetName() const { return m_name; }

	glm::vec3 GetPosition() const { return m_pos; }
	void SetPosition(const glm::vec3& pos) { m_pos = pos; }

	void SetRenderPass(int pass) { m_renderPass = pass; }
	int GetRenderPass() const { return m_renderPass; }

	GLuint GetShaderProg() const { return m_shaderProg; }
	unsigned int GetRP() const { return m_renderPass; }

protected:

	string m_name;
	string m_type;

	GLuint m_shaderID = 0;
	GLuint m_shaderProg;

	glm::vec3 m_pos = glm::vec3(0.0f);
	int m_renderPass = 0;

	Scene* m_scene = nullptr;
};


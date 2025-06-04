#pragma once
#include "core.h"
#include <string>
#include "Shader.h"
#include "Texture.h"
#include "Model.h"

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

	void SetName(const std::string& name) { m_name = name; }
	std::string GetName() const { return m_name; }

	glm::vec3 GetPosition() const { return m_pos; }
	void SetPosition(const glm::vec3& pos) { m_pos = pos; }

	void SetRenderPass(int pass) { m_renderPass = pass; }
	int GetRenderPass() const { return m_renderPass; }

	Shader* GetShader() const { return m_shader; }
	Texture* GetTexture() const { return m_texture; }
	Model* GetModel() const { return m_model; }

	unsigned int GetRP() const { return m_renderPass; }

protected:

	string m_name;
	string m_type;

	glm::vec3 m_pos = glm::vec3(0.0f);
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;

	int m_renderPass = 0;

	Scene* m_scene = nullptr;
	Shader* m_shader = nullptr;
	Texture* m_texture = nullptr;
	Model* m_model = nullptr;

	string m_modelName;
	string m_textureName;
	string m_shaderName;
};


#pragma once
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <string>
#include <fstream>

class Terrain : public GameObject
{
public:
	Terrain();
	~Terrain();

	void Load(std::ifstream& _file);
	void Render();
	void PreRender();

	void SetModel(Model* m) { m_model = m; }
	void SetTexture(Texture* t) { m_texture = t; }
	void SetShader(Shader* s) { m_shader = s; }

	Shader* GetShader() { return m_shader; }

private:
	std::string m_name;
	Model* m_model = nullptr;
	Texture* m_texture = nullptr;
	Shader* m_shader = nullptr;

	glm::vec3 m_pos;
	glm::vec3 m_scale;
	glm::mat4 m_worldMatrix;
	GLuint m_shaderProg = 0;
};
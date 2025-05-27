#pragma once

#include "Model.h"
#include "GameObject.h"
#include "core.h"

class Cube: public Model, public GameObject
{
public:

	Cube();
	virtual ~Cube();

	void Init(Scene* scene) override;
	void Render() override;

	void Load(std::ifstream& file) override;
	void render();
private:

	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;

	void setupMesh();
};
#pragma once

#include "Model.h"
#include "GameObject.h"
#include "core.h"

class Cube: public Model, public GameObject {

private:

	string m_type;

	GLuint				m_numFaces = 0;
	GLuint				m_vao = 0;

	GLuint				m_texCoordBuffer;
	GLuint				m_vertexBuffer;
	GLuint				m_colourBuffer;
	GLuint				m_indexBuffer;
	GLuint				m_normalBuffer;

public:

	Cube();
	~Cube();

	void render();
};
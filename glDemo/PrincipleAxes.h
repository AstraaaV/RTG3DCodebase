#pragma once

#include "core.h"
#include "Model.h"
#include "GameObject.h"


class CGPrincipleAxes : public Model, public GameObject {

private:

	GLuint				m_numFaces = 0;
	GLuint				m_vao = 0;

	GLuint				m_vertexBuffer;
	GLuint				m_colourBuffer;
	GLuint				m_indexBuffer;

public:

	CGPrincipleAxes();
	~CGPrincipleAxes();

	void render(bool _showZAxis = true);
};

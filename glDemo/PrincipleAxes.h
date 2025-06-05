#pragma once

#include "core.h"



class CGPrincipleAxes {

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
#include "PrincipleAxes.h"


using namespace std;
using namespace glm;


// Example data for principle axes

// Packed vertex buffer for principle axes model
static float positionArray[] = {

	// x axis model
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.025f, 0.025f, 0.0, 1.0f,
	1.075f, 0.075f, 0.0, 1.0f,
	1.025f, 0.075f, 0.0, 1.0f,
	1.075f, 0.025f, 0.0, 1.0f,

	// y axis model
	0.0, 0.0, 0.0, 1.0f,
	0.0, 1.0, 0.0, 1.0f,
	-0.075f, 1.075f, 0.0, 1.0f,
	-0.05f, 1.05f, 0.0, 1.0f,
	-0.025f, 1.075f, 0.0, 1.0f,
	-0.075f, 1.025f, 0.0, 1.0f,

	// z axis model
	0.0, 0.0, 0.0, 1.0f,
	0.0, 0.0, 1.0, 1.0f,
	0.025f, 0.075f, 1.0, 1.0f,
	0.075f, 0.075f, 1.0, 1.0f,
	0.025f, 0.025f, 1.0, 1.0f,
	0.075f, 0.025f, 1.0, 1.0f
};

// Packed colour buffer for principle axes model
static float colourArray[] = {

	// x axis colour (red)
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	// y axis colour (green)
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	// z axis colour (blue)
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f
};


// Line list topology to render principle axes
static unsigned int indexArray[] = {

	0, 1, 2, 3, 4, 5,					// x axis
	6, 7, 8, 9, 10, 11,					// y axis
	12, 13, 14, 15, 15, 16, 16, 17		// z axis
};



CGPrincipleAxes::CGPrincipleAxes() {


	m_numFaces = 10;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// setup vbo for position attribute
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), positionArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	// setup vbo for colour attribute
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), colourArray, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(4);

	// setup vbo for cube) index buffer
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 20 * sizeof(unsigned int), indexArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
}


CGPrincipleAxes::~CGPrincipleAxes() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_colourBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}


void CGPrincipleAxes::render(bool _showZAxis) {
	glBindVertexArray(m_vao);

	if (_showZAxis)
	{
		glDrawElements(GL_LINES, m_numFaces * 3, GL_UNSIGNED_INT, (const GLvoid*)0);
	}
	else
	{
		glDrawElements(GL_LINES, (m_numFaces - 2) * 3, GL_UNSIGNED_INT, (const GLvoid*)0);
	}
}

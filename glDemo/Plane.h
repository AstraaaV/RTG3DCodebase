#pragma once
#include "core.h"
#include "Model.h"

class Plane : public Model
{
public:
	Plane();
	~Plane();

	void Render();

private:
	GLuint m_vao = 0;
	GLuint m_vertexBuffer = 0;
	GLuint m_colourBuffer = 0;
	GLuint m_indexBuffer = 0;
	GLuint m_uvBuffer = 0;
	int m_numFaces = 0;
};
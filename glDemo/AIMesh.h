#pragma once

#include "core.h"
#include <string>

class AIMesh {
public:
	AIMesh(std::string _filename, GLuint _meshIndex = 0);
	~AIMesh();

	void addTexture(GLuint _textureID);
	void addTexture(std::string _filename, FREE_IMAGE_FORMAT _format);

	void addNormalMap(GLuint _normalMapID);
	void addNormalMap(std::string _filename, FREE_IMAGE_FORMAT _format);

	void setupTextures();
	void render();

private:

	GLuint				m_numFaces = 0;

	GLuint				m_vao = 0;

	GLuint				m_meshVertexPosBuffer = 0;
	GLuint				m_meshTexCoordBuffer = 0;

	GLuint				m_meshNormalBuffer = 0; // surface basis z
	GLuint				m_meshTangentBuffer = 0; // surface basis x (u aligned)
	GLuint				m_meshBiTangentBuffer = 0; // surface basis y (v aligned)

	GLuint				m_meshFaceIndexBuffer = 0;

	GLuint				m_textureID = 0;
	GLuint				m_normalMapID = 0;
};
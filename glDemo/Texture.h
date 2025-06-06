#pragma once
#include "core.h"
#include <string>
#include <fstream>

//simple data structure that loads a texture using FreeImage
//from its description in the manifest and then links its GLuint handle to its name
class Texture
{
public:
	explicit Texture(std::ifstream& _file);
	~Texture();

	GLuint GetTexID() { return m_texID; }
	const std::string GetName() const { return m_name; }

	GLuint m_texID = 0;

protected:
	std::string m_name;

};

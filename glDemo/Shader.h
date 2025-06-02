#pragma once
#include "core.h"
#include <string>
#include <fstream>

//simple data structure that loads and compiles a shader
//from its description in the manifest and then links its GLuint handle to its name
class Shader
{
public:
	Shader(std::ifstream& _file);
	~Shader();

	GLuint GetProg() const { return m_shaderProg; }
	const std::string& GetName() const { return m_name; }

protected:
	std::string m_name;
	GLuint m_shaderProg = 0;

};

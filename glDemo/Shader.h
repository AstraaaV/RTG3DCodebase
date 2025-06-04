#pragma once
#include "core.h"
#include <string>

using namespace std;

//simple data structure that loads and compiles a shader
//from its description in the manifest and then links its GLuint handle to its name
class Shader
{
public:
	Shader(ifstream& _file);
	Shader(const std::string& name, const std::string& vertFile, const std::string& fragFile);
	~Shader();

	GLuint GetProg() { return m_shaderProg; }
	string GetName() { return m_name; }

protected:
	string m_name;
	GLuint m_shaderProg;
	string m_vertFilename;
	string m_fragFilename;

};
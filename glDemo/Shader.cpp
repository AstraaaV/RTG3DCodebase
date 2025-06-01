#include "Shader.h"
#include "shader_setup.h"
#include "stringHelp.h"

Shader::Shader(ifstream& _file)
{
	string fileNameV,fileNameF;
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::String(_file, "VERTFILE", fileNameV);
	StringHelp::String(_file, "FRAGFILE", fileNameF);

	m_shaderProg = setupShaders(fileNameV, fileNameF);

	printf("SHADER LOADED: %s\n", m_name.c_str());
}

Shader::~Shader()
{
}

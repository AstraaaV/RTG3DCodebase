#include "Shader.h"
#include "shader_setup.h"
#include "stringHelp.h"

Shader::Shader(ifstream& _file)
{
	string fileNameV, fileNameF;
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::String(_file, "VERTFILE", fileNameV);
	StringHelp::String(_file, "FRAGFILE", fileNameF);

	m_shaderProg = setupShaders(fileNameV, fileNameF);

	GLint success;
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_shaderProg, 512, NULL, infoLog);
		cout << "[SHADER LINK ERROR]: " << infoLog << endl;
	}
}

Shader::~Shader()
{
}
#include "Shader.h"
#include "shader_setup.h"
#include "stringHelp.h"

inline std::string Trim(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos) return "";
	size_t last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, last - first + 1);
}

Shader::Shader(ifstream& _file)
{
	string fileNameV, fileNameF;
	StringHelp::String(_file, "NAME", m_name);
	m_name = Trim(m_name);

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

Shader::Shader(const std::string& name, const std::string& vertFile, const std::string& fragFile)
{
	m_name = name;
	m_vertFilename = vertFile;
	m_fragFilename = fragFile;

	m_shaderProg = setupShaders(m_vertFilename, m_fragFilename);

	GLint success;
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_shaderProg, 512, NULL, infoLog);
	}

}

Shader::~Shader()
{
}
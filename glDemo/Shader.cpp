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

	if (m_shaderProg == 0)
	{
		cout << "Shader failed: " << m_name << "\n"
			<< " -> VERT: " << fileNameV << "\n"
			<< " -> FRAG: " << fileNameF << endl;
	}
	else
	{
		cout << "Shader loaded: " << m_name << endl;
	}
}

Shader::~Shader()
{
}

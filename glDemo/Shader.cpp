#include "Shader.h"
#include "shader_setup.h"
#include "stringHelp.h"

Shader::Shader(ifstream& _file)
{
	string fileNameV,fileNameF;
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::String(_file, "VERTFILE", fileNameV);
	StringHelp::String(_file, "FRAGFILE", fileNameF);

	if (m_name == "TEXPOINT")
	{
		fileNameV = "Assets\\Shaders\\texture-pointlight.vert";
		fileNameF = "Assets\\Shaders\\texture-pointlight.frag";
	}
	else if (m_name == "SUNLIGHT")
	{
		fileNameV = "Assets\\Shaders\\texture-directional.vert";
		fileNameF = "Assets\\Shaders\\texture-directional.frag";
	}

	m_shaderProg = setupShaders(fileNameV, fileNameF);

	if (m_shaderProg == 0)
	{
		std::cout << "\n[CRITICAL ERROR] Shader failed: " << m_name << "\n"
			<< " -> VERT: " << fileNameV << "\n"
			<< " -> FRAG: " << fileNameF << endl;

		assert(false && "shader compile/llink failed. see error log above.");
	}
	else
	{
		std::cout << "Shader loaded: " << m_name << endl;
	}
}

Shader::~Shader()
{
}

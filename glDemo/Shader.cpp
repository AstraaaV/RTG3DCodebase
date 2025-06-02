#include "Shader.h"
#include "shader_setup.h"
#include "stringHelp.h"
#include <iostream>

Shader::Shader(std::ifstream& _file)
{
	string fileNameV,fileNameF;

	StringHelp::String(_file, "NAME", m_name);
	StringHelp::String(_file, "VERTFILE", fileNameV);
	StringHelp::String(_file, "FRAGFILE", fileNameF);

	/*if (m_name == "TEXPOINT")
	{
		fileNameV = "Assets\\Shaders\\texture-pointlight.vert";
		fileNameF = "Assets\\Shaders\\texture-pointlight.frag";
	}
	else if (m_name == "SUNLIGHT")
	{
		fileNameV = "Assets\\Shaders\\texture-directional.vert";
		fileNameF = "Assets\\Shaders\\texture-directional.frag";
	}*/

	std::cout << "\n[SHADER] Attempting to load shader: " << m_name << endl;
	std::cout << "VERT FILE: " << fileNameV << endl;
	std::cout << "FRAG FILE: " << fileNameF << endl;

	m_shaderProg = setupShaders(fileNameV, fileNameF);

	if (m_shaderProg == 0)
	{
		std::cout << "\n[CRITICAL ERROR] Shader failed: " << m_name << "\n"
			<< " -> VERT: " << fileNameV << "\n"
			<< " -> FRAG: " << fileNameF << std::endl;

		assert(false && "Shader compile/link failed. See error log above.");
	}
	else
	{
		std::cout << "Shader loaded: " << m_name << std::endl;
	}
}

Shader::~Shader()
{
}

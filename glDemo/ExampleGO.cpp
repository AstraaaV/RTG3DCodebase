#include "ExampleGO.h"
#include "AIModel.h"
#include "stringHelp.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "helper.h"

ExampleGO::ExampleGO()
{
}

ExampleGO::~ExampleGO()
{
}

void ExampleGO::Load(std::istream& _file)
{
	GameObject::Load(_file);

	StringHelp::String(_file, "MODEL", m_ModelName);
	StringHelp::String(_file, "TEXTURE", m_TexName);
	StringHelp::String(_file, "SHADER", m_ShaderName);

	if (!m_ShaderName.empty())
	{
		m_shader = m_scene->GetShader(m_ShaderName);
		if (!m_shader)
			cout << "[ExampleGO::Load] Shader \"" << m_ShaderName << "\" not found.\n";
	}

	if (!m_TexName.empty())
	{
		m_texture = m_scene->GetTexture(m_TexName);
		if (!m_texture)
			cout << "[ExampleGO::Load] Texture \"" << m_TexName << "\" not found.\n";
	}

	if (!m_ModelName.empty())
	{
		m_model = m_scene->GetModel(m_ModelName);
		if (!m_model)
			cout << "[ExampleGO::Load] Texture \"" << m_ModelName << "\" not found.\n";
	}
}

void ExampleGO::Tick(float _dt, GLFWwindow* window)
{
	GameObject::Tick(_dt, window);
}

void ExampleGO::PreRender()
{
	if (!m_shader) return;

	GLuint prog = m_shader->GetProg();
	glUseProgram(prog);
	//only thing I need to do is tell the shader about my texture

	if (m_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTexID());

		GLint texLoc = glGetUniformLocation(prog, "diffuseTex");
		glUniform1i(texLoc, 0);
	}

	float time = glfwGetTime();
	GLint timeLoc = glGetUniformLocation(prog, "u_time");
	glUniform1f(timeLoc, time);

	//TODO: this does sort of replicate stuff in the AIMesh class, could we make them more compatible.

	//TODO: NORMAL MAPS!

	GLuint m_normalMap{};

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalMap);

	GLint normLoc;
	Helper::SetUniformLocation(m_shader->GetProg(), "u_normalMap", &normLoc);
	glUniform1i(normLoc, 1);
}

void ExampleGO::Render()
{
	if (!m_model)
	{
		cout << "[ExampleGO::Render] Warning: m_model is null.\n";
		return;
	}
	m_model->Render();
}

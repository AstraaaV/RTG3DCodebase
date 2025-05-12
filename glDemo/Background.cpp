#include "Background.h"
#include "Scene.h"
#include "stringHelp.h"
#include "helper.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"

Background::Background()
{
}

Background::~Background()
{
}

void Background::Load(ifstream& _file)
{
	GameObject::Load(_file);
	StringHelp::String(_file, "MODEL", m_ModelName);
	StringHelp::String(_file, "TEXTURE", m_TexName);
	StringHelp::String(_file, "SHADER", m_ShaderName);
}

void Background::Tick(float _dt, GLFWwindow* window)
{
	GameObject::Tick(_dt, window);
}

void Background::PreRender()
{
	GameObject::PreRender();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	GLint timeLoc;
	Helper::SetUniformLocation(m_ShaderProg, "u_time", &timeLoc);
	glUniform1f(timeLoc, glfwGetTime());
}

void Background::Render()
{
	m_model->Render();
}

void Background::Init(Scene* _scene)
{
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	m_texture = _scene->GetTexture(m_TexName)->GetTexID();
	m_model = _scene->GetModel(m_ModelName);

	SetRenderPass(RP_BACKGROUND); // Makes sure its in bg render pass

	std::cout << "Texture ID: " << m_texture << std::endl;
}

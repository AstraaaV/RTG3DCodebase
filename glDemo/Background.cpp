#include "Background.h"
#include "Scene.h"
#include "stringHelp.h"
#include "helper.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include <RenderPass.h>

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
	if (m_texture != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	if (m_shader != 0)
	{
		GLuint shaderID = m_shader->GetProg();
		GLint timeLoc;
		Helper::SetUniformLocation(shaderID, "u_time", &timeLoc);
		glUniform1f(timeLoc, glfwGetTime());
	}
}

void Background::Render()
{
	if(m_model)
		m_model->Render();
}

void Background::Init(Shader* shader, GLuint texture, Model* model)
{
	m_shader = shader;
	SetShader(shader);

	m_texture = texture;
	m_model = model;

	SetRenderPass(RP_BACKGROUND); // Makes sure its in bg render pass

	std::cout << "Texture ID: " << m_texture << std::endl;

	if (!m_texture || !m_shader || !m_model)
		std::cout << "[BACKGROUND] Init Warning: Missing Asset(s): "
		<< "Shader = " << m_shader
		<< ", Texture = " << m_texture
		<< ", Model = " << (m_model ? "OK" : "NULL") << endl;
}

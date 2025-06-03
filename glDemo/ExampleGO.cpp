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

void ExampleGO::Load(ifstream& _file)
{
	GameObject::Load(_file);
	StringHelp::String(_file, "MODEL", m_ModelName);
	StringHelp::String(_file, "TEXTURE", m_TexName);
	StringHelp::String(_file, "SHADER", m_ShaderName);

}

void ExampleGO::Tick(float _dt, GLFWwindow* window)
{
	GameObject::Tick(_dt, window);
}

void ExampleGO::PreRender()
{
	//only thing I need to do is tell the shader about my texture

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	GLint pLocation;
	float time = glfwGetTime();
	Helper::SetUniformLocation(m_shader, "u_time", &pLocation);
	glUniform1f(pLocation, time);

	//TODO: this does sort of replicate stuff in the AIMesh class, could we make them more compatible.

	//TODO: NORMAL MAPS!

	GLuint m_normalMap{};

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalMap);

	GLint normLoc;
	Helper::SetUniformLocation(m_shader, "u_normalMap", &normLoc);
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

void ExampleGO::Init(Scene* _scene, GLuint shaderProg, GLuint textureID, Model* model)
{
	m_shader = shaderProg;
	m_texture = textureID;
	m_model = model;
}

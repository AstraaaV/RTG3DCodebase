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
	GameObject::PreRender();

	//only thing I need to do is tell the shader about my texture

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	GLint pLocation;
	float time = glfwGetTime();
	Helper::SetUniformLocation(m_ShaderProg, "u_time", &pLocation);
	glUniform1f(pLocation, time);

	//TODO: this does sort of replicate stuff in the AIMesh class, could we make them more compatible.

	//TODO: NORMAL MAPS!

	GLuint m_normalMap{};

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalMap);

	GLint normLoc;
	Helper::SetUniformLocation(m_ShaderProg, "u_normalMap", &normLoc);
	glUniform1i(normLoc, 1);
}

void ExampleGO::Render()
{
	m_model->Render();
}

void ExampleGO::Init(Scene* _scene)
{
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	m_texture = _scene->GetTexture(m_TexName)->GetTexID();
	m_model = _scene->GetModel(m_ModelName);
}

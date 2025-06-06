#include "core.h"
#include "GameObject.h"
#include "stringHelp.h"
#include "Shader.h"
#include "Scene.h"
#include "Texture.h"
#include "helper.h"

using namespace glm;

GameObject::GameObject()
{
	m_type = "GAMEOBJECT";
}

GameObject::GameObject(const std::string& name, const std::string& model, const std::string& texture, const std::string& shader)
{
	m_name = name;
	m_modelName = model;
	m_textureName = texture;
	m_shaderName = shader;
	m_pos = glm::vec3(0.0f);
	m_rot = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_RP = RP_OPAQUE;
}

GameObject::~GameObject()
{
}

void GameObject::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "ROT", m_rot.x, m_rot.y, m_rot.z);
	StringHelp::Float3(_file, "SCALE", m_scale.x, m_scale.y, m_scale.z);
	StringHelp::Float3(_file, "ROT INC", m_rot_incr.x, m_rot_incr.y, m_rot_incr.z);
}

void GameObject::Tick(float _dt)
{
	m_rot += m_rot_incr;

	m_worldMatrix = glm::translate(mat4(1.0), vec3(m_pos));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));
}

void GameObject::PreRender()
{
	// Setup model transform
	GLint pLocation;
	Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&m_worldMatrix);

	if (m_shaderName == "TEXWALL")
	{
		Texture* texDiffuse = m_scene->GetTexture(m_textureName);
		if (texDiffuse)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texDiffuse->m_texID);
			glUniform1i(glGetUniformLocation(m_ShaderProg, "diffuseMap"), 0);
		}
	
		Texture* texNormal = m_scene->GetTexture(m_textureName2);
		if (texNormal)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texNormal->m_texID);
			glUniform1i(glGetUniformLocation(m_ShaderProg, "normalMap"), 1);
		}

		Texture* texRoughness = m_scene->GetTexture(m_textureName3);
		if (texRoughness)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texRoughness->m_texID);
			glUniform1i(glGetUniformLocation(m_ShaderProg, "roughnessMap"), 2);
		}
	
	}
}

void GameObject::Render()
{
	//I have nothing to draw
}

void GameObject::SetShader(Shader* _shader)
{
	m_shader = _shader;
}

void GameObject::Init(Scene* _scene)
{
	//I have nothing to link up to
}
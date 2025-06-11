#include "core.h"
#include "GameObject.h"
#include "stringHelp.h"
#include "Shader.h"
#include "Scene.h"
#include "Texture.h"
#include "helper.h"
#include "Model.h"

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
	m_baseY = m_pos.y;
	StringHelp::Float3(_file, "ROT", m_rot.x, m_rot.y, m_rot.z);
	StringHelp::Float3(_file, "SCALE", m_scale.x, m_scale.y, m_scale.z);
	StringHelp::Float3(_file, "ROT INC", m_rot_incr.x, m_rot_incr.y, m_rot_incr.z);
}

void GameObject::Tick(float _dt)
{
	m_rot += m_rot_incr;

	if (m_name == "BEAST")
	{
		float bob = sinf(glfwGetTime() * 2.0f) * 0.1f;
		m_pos.y = m_baseY + bob;
	}

	m_worldMatrix = glm::translate(mat4(1.0), vec3(m_pos));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));
}

void GameObject::PreRender()
{
	if (!m_ShaderProg) return;

	// Setup model transform
	GLint pLocation;
	Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&m_worldMatrix);

	if (!m_textureName.empty())
	{
		Texture* texDiffuse = m_scene->GetTexture(m_textureName);
		if (texDiffuse)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texDiffuse->m_texID);
			pLocation = glGetUniformLocation(m_ShaderProg, "u_BaseColor");
			if (pLocation != -1) glUniform1i(pLocation, 0);
		}

		Texture* texNormal = m_scene->GetTexture(m_textureName2);
		if (texNormal)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texNormal->m_texID);
			pLocation = glGetUniformLocation(m_ShaderProg, "u_NormalMap");
			if (pLocation != -1) glUniform1i(pLocation, 1);
		}

		Texture* texRoughness = m_scene->GetTexture(m_textureName3);
		if (texRoughness)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texRoughness->m_texID);
			pLocation = glGetUniformLocation(m_ShaderProg, "u_RoughnessMap");
			if (pLocation != -1) glUniform1i(pLocation, 2);
		}
	
		Texture* texMetallic = m_scene->GetTexture(m_textureName4);
		if (texMetallic)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, texMetallic->m_texID);
			pLocation = glGetUniformLocation(m_ShaderProg, "u_MetallicMap");
			if (pLocation != -1) glUniform1i(pLocation, 3);
		}

		Texture* texHeight = m_scene->GetTexture(m_textureName5);
		if (texHeight)
		{
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, texHeight->m_texID);
			pLocation = glGetUniformLocation(m_ShaderProg, "u_HeightMap");
			if (pLocation != -1) glUniform1i(pLocation, 4);
		}

		Texture* texFlat = m_scene->GetTexture(m_textureName);
		if (texFlat)
		{
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, texFlat->m_texID);
			pLocation = glGetUniformLocation(m_ShaderProg, "u_Diffuse");
			if (pLocation != -1) glUniform1i(pLocation, 5);
		}
	}
}

void GameObject::Render()
{
	if (!m_visible)
		return;

	Model* model = m_scene->GetModel(m_modelName);

	if (m_RP == RP_TRANSPARENT)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	model->Render();

	if (m_RP == RP_TRANSPARENT)
	{
		glDisable(GL_BLEND);
	}
}

void GameObject::SetShader(Shader* _shader)
{
	m_shader = _shader;
}

void GameObject::SetRotation(const glm::vec3& rot)
{
	m_rot = rot;
}

const glm::vec3& GameObject::GetRotation() const
{
	return m_rot;
}

void GameObject::SetScale(const glm::vec3& scale)
{
	m_scale = scale;
}

void GameObject::SetRenderPass(RenderPass rp)
{
	m_RP = rp;
}

void GameObject::SetVisible(bool visible)
{
	m_visible = visible;
}

bool GameObject::IsVisible() const
{
	return m_visible;
}

void GameObject::Init(Scene* _scene)
{
	m_scene = _scene;

	Model* model = _scene->GetModel(m_modelName);
	m_shader = _scene->GetShader(m_shaderName);

	if (m_shader)
		m_ShaderProg = m_shader->GetProg();

	if (m_shaderName == "TEXWALL" && m_modelName == "WALL")
	{
		m_textureName2 = "WALL_NORMAL";
		m_textureName3 = "WALL_ROUGHNESS";
	}
	else if (m_modelName == "WALLSCONCE")
	{
		m_textureName = "WALLSCONCE_BASE";
		m_textureName2 = "WALLSCONCE_NORMAL";
		m_textureName3 = "WALLSCONCE_ROUGHNESS";
		m_textureName4 = "WALLSCONCE_METALLIC";
		m_textureName5 = "WALLSCONCE_HEIGHT";
	}
}
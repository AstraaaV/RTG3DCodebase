#include "core.h"
#include "GameObject.h"
#include "stringHelp.h"
#include <sstream>
#include "helper.h"
#include "Scene.h"

using namespace glm;

GameObject::GameObject()
{
	m_type = "GAMEOBJECT";
}

GameObject::~GameObject()
{
}

void GameObject::Load(ifstream& _file)
{
	string line;
	while (std::getline(_file, line))
	{
		if (line.find("}") != string::npos)
			break;

		std::istringstream iss(line);
		string key;
		iss >> key;

		if (key == "NAME")
			iss >> m_name;
		else if (key == "POS")
			iss >> m_pos.x >> m_pos.y >> m_pos.z;
		else if (key == "ROT")
			iss >> m_rot.x >> m_rot.y >> m_rot.z;
		else if (key == "SCALE")
			iss >> m_scale.x >> m_scale.y >> m_scale.z;
		else if (key == "ROTINC")
			iss >> m_rot_incr.x >> m_rot_incr.y >> m_rot_incr.z;
		else if (key == "MODEL")
		{
			string modelName;
			iss >> modelName;
			m_modelName = modelName;
		}
		else if (key == "TEXTURE")
		{
			string texName;
			iss >> texName;
			m_textureName = texName;
		}
		else if (key == "SHADER")
		{
			string shaderName;
			iss >> shaderName;
			m_shaderName = shaderName;
		}
	}
	cout << "[DEBUG] GO loaded: " << m_name << "\n";

	if (m_modelName.empty()) cout << "[WARN] " << m_name << " has no model assigned!\n";
	if (m_textureName.empty()) cout << "[WARN] " << m_name << " has no texture assigned!\n";
	if (m_shaderName.empty()) cout << "[WARN] " << m_name << " has no shader assigned!\n";
}

void GameObject::Tick(float _dt)
{
	m_rot += m_rot_incr;

	m_worldMatrix = glm::translate(mat4(1.0), vec3(m_pos));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_worldMatrix = glm::rotate(m_worldMatrix, glm::radians(m_rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_worldMatrix = glm::scale(m_worldMatrix, glm::vec3(m_scale));

	if (m_isAnimated)
	{
		m_animTime += _dt;

		vec3 animatedPos = m_basePos;
		animatedPos.y += sin(m_animTime * 2.0f) * 0.2f;
		m_pos = animatedPos;
	}

	if (m_isPacing)
	{
		m_paceTime += _dt;

		float offset = sin(m_paceTime * m_paceSpeed) * m_paceDistance;
		m_pos = m_initialPacePos + m_paceDirection * offset;
	}
}

void GameObject::PreRender()
{
	// Setup model transform
	GLint pLocation;
	Helper::SetUniformLocation(m_ShaderProg, "modelMatrix", &pLocation);
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&m_worldMatrix);

	glUseProgram(m_ShaderProg);

	if (m_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTexID());

		GLint texLoc;
		Helper::SetUniformLocation(m_ShaderProg, "tex", &texLoc);
		glUniform1i(texLoc, 0);
	}
}

void GameObject::Render()
{
	if (m_model)
	{
		m_model->Render();
	}
}

void GameObject::EnablePacing(const glm::vec3& direction, float distance, float speed)
{
	m_isPacing = true;
	m_paceDirection = direction;
	m_paceDistance = distance;
	m_paceSpeed = speed;
	m_initialPacePos = m_pos;
}

void GameObject::Init(Scene* _scene)
{
	m_scene = _scene;

	if (!m_model && !m_modelName.empty())
		m_model = _scene->GetModel(m_modelName);

	if (!m_texture && !m_textureName.empty())
		m_texture = _scene->GetTexture(m_textureName);

	if (!m_shader && !m_shaderName.empty())
		m_shader = _scene->GetShader(m_shaderName);

	if (m_shader)
		m_ShaderProg = m_shader->GetProg();
}
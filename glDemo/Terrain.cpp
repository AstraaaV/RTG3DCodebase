#include "Terrain.h"
#include "helper.h"
#include "stringHelp.h"

using namespace glm;

Terrain::Terrain() {}

Terrain::~Terrain() {}

void Terrain::Load(std::ifstream& _file)
{
	std::string dummy;
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "SCALE", m_scale.x, m_scale.y, m_scale.z);
}

void Terrain::Render()
{
	if (m_model)
		m_model->Render();
}

void Terrain::PreRender()
{
	m_shaderProg = m_shader->GetProg();

	m_worldMatrix = translate(mat4(1.0f), m_pos);
	m_worldMatrix = scale(m_worldMatrix, m_scale);

	GLint loc;
	if (Helper::SetUniformLocation(m_shaderProg, "modelMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)&m_worldMatrix);

	if (m_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTexID());

		if (Helper::SetUniformLocation(m_shaderProg, "tex", &loc))
			glUniform1i(loc, 0);
	}
}



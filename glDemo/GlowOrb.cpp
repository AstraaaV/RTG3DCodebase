#include "GlowOrb.h"
#include "Light.h"
#include "Scene.h"

GlowOrb::GlowOrb()
{
	SetName("GLOWORB");
	SetRenderPass(RP_TRANSPARENT);
}

GlowOrb::~GlowOrb()
{
	if (m_scene && m_orbLight)
	{
		delete m_orbLight;
		m_orbLight = nullptr;
	}
}

void GlowOrb::Init(Scene* scene)
{
	GameObject::Init(scene);
	m_modelName = "SPHERE";
	SetTexture("FIRE_DIFFUSE");
	m_shaderName = "EMISSIVE";
	m_emissiveShader = scene->GetShader(m_shaderName);

	if (m_shader)
		m_ShaderProg = m_shader->GetProg();

	m_orbLight = new Light();
	m_orbLight->SetType("POINT");
	m_orbLight->SetName(GetName() + "_LIGHT");
	m_orbLight->SetPos(GetPosition());
	m_orbLight->SetColour(glm::vec3(1.0f, 0.6f, 0.2f));
	m_orbLight->SetAmbient(glm::vec3(0.1f));
	m_orbLight->SetAttenuation(0.6f, 0.045f, 0.0075f);

	m_scene->AddLight(m_orbLight);
}

void GlowOrb::Tick(float dt)
{
	GameObject::Tick(dt);

	float time = glfwGetTime();
	glm::vec3 pos = GetPosition();
	pos.y = sin(time * 2.0f) * 0.2f + 1.5f;
	SetPosition(pos);

	if (m_orbLight)
		m_orbLight->SetPos(pos);

	if (m_emissiveShader)
	{
		GLuint prog = m_emissiveShader->GetProg();
		glUseProgram(prog);

		int frame = static_cast<int>(time * 15.0f) % 64;
		GLint frameLoc = glGetUniformLocation(prog, "frame");
		
		if (frameLoc != -1)
		{
			glUniform1i(frameLoc, frame);
		}

		glUseProgram(0);
	}

}

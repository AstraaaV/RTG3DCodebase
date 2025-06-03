#include "core.h"
#include "GameObject.h"
#include "stringHelp.h"
#include "Scene.h"
#include "helper.h"
#include "Camera.h"
#include <RenderPass.h>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

GameObject::GameObject()
{
	m_name = "GameObject";
	m_pos = glm::vec3(0.0f);
	m_renderPass = 0;
	m_scene = nullptr;
}

GameObject::~GameObject()
{
}

void GameObject::Init(Scene* scene)
{
	m_scene = scene;

	float vertices[] =
	{
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
	};

	unsigned int indices[] =
	{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		0, 1, 5, 5, 4, 0,
		2, 3, 7, 7, 6, 2,
		0, 3, 7, 7, 4, 0,
		1, 2, 6, 6, 5, 1
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void GameObject::Tick(float deltaTime, GLFWwindow* window)
{
}

void GameObject::Render()
{
	if (!m_shader) return;

	GLuint shaderProg = m_shader->GetProg();
	glUseProgram(shaderProg);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_pos);
	glm::mat4 view = m_scene->GetActiveCamera()->GetView();
	glm::mat4 proj = m_scene->GetActiveCamera()->GetProj();

	GLint modelLoc = glGetUniformLocation(shaderProg, "model");
	GLint viewLoc = glGetUniformLocation(shaderProg, "view");
	GLint projLoc = glGetUniformLocation(shaderProg, "proj");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	if (m_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture->GetTexID());
		GLint texLoc = glGetUniformLocation(shaderProg, "diffuseTex");
		glUniform1i(texLoc, 0);
	}

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GameObject::Load(std::ifstream& file)
{
	StringHelp::String(file, "NAME", m_name);
	StringHelp::Float3(file, "POS", m_pos.x, m_pos.y, m_pos.z);

	int shaderIndex = -1;
	StringHelp::Int(file, "SHADER", shaderIndex);

	if (shaderIndex >= 0)
	{
		Shader* myShader = m_scene->GetShader("TEXDIR");

		if (myShader)
		{
			m_shader = myShader;
		}
		else
		{
			cout << "[GameObject::Load] Warning: Shader index " << shaderIndex << " not found.\n";
		}
	}
	else
	{
		cout << "[GameObject::Load] Warning: Shader index missing or invalid for " << m_name << "\n";
	}
	m_renderPass = RP_OPAQUE;
}

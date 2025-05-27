#include "core.h"
#include "GameObject.h"
#include "stringHelp.h"
#include "helper.h"

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
}

void GameObject::Tick(float deltaTime, GLFWwindow* window)
{
}

void GameObject::Render()
{
}

void GameObject::Load(std::ifstream& file)
{
	StringHelp::String(file, "NAME", m_name);
	StringHelp::Float3(file, "POS", m_pos.x, m_pos.y, m_pos.z);
}

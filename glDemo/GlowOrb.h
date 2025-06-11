#pragma once
#include "GameObject.h"

class Light;

class GlowOrb : public GameObject
{
public:
	GlowOrb();
	~GlowOrb();

	virtual void Init(Scene* scene) override;
	virtual void Tick(float dt) override;

private:
	Light* m_orbLight = nullptr;
	Shader* m_emissiveShader;
};
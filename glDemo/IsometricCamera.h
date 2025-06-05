#pragma once
#include "Camera.h"

class IsometricCamera : public Camera
{
public:
	IsometricCamera();
	void Init(float width, float height, Scene* scene) override;
	void Tick(float dt) override;

private:
	void updateCam();
};
#pragma once
#include "WindowPhysics.h"
#include <random>
#include <__msvc_chrono.hpp>

class Movements
{
public:
	float speed = 500;
	POINT WanderTarget;
	Movements(WindowPhysics* b);
	~Movements();

	void MoveTo(POINT target);
	void GenerateWanderTarget();
	void Jump(float jumpForce);
	void RunFrom(POINT target, float speed);
	void MovingMode(bool isClimbing);

private:
	WindowPhysics* movingBody;

	int jumpTimer = 1000;
	DWORD lastJump = 0;


	bool CanJump();
	bool IsOnGround();
};


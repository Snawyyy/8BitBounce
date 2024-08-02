#pragma once
#include "WindowPhysics.h"
#include "Movements.h"

class Creature
{
public:
	Creature(HWND hWnd);
	~Creature();

	void Move();
	void Wandering();

private:
	WindowPhysics* creatureBody;
	Movements* movements;
};
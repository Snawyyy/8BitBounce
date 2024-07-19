#include "Creature.h"

Creature::Creature(HWND hWnd)
{
	creatureBody = new WindowPhysics(hWnd);
}

Creature::~Creature()
{
	delete creatureBody;
}

void Creature::Move() 
{
	creatureBody->RunPhysics();
}

void Creature::Wandering()
{
	if (creatureBody->body.pos.x == movements->WanderTarget.x)
	{
		movements->GenerateWanderTarget();
	}
	movements->MoveTo(movements->WanderTarget);
}

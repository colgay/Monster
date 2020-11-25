#pragma once

#include "BaseMonster.h"

class Zombie : public BaseMonster
{
	virtual void Precache() override;
	virtual const char *GetName() override { return "Zombie"; }
};
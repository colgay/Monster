#pragma once

#include "extdll.h"

class MonsterBase
{
public:
	MonsterBase(edict_t* pEntity)
	{
		m_pEdict = pEntity;
	}

	virtual ~MonsterBase() {};

	virtual void Precache() = 0;
	virtual void Spawn(const Vector& origin, const Vector& angle) = 0;
	virtual void Think() = 0;

	edict_t* GetEdict() const { return m_pEdict; }

protected:
	edict_t* m_pEdict;
};
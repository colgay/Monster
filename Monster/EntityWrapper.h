#pragma once

#include "extdll.h"

#include "MonsterManager.h"

class EntityWrapper
{
public:
	EntityWrapper(edict_t* pEntity) : m_pEdict(pEntity) {}

	bool IsMonster() const { return GetMonsterManager().GetMonster(m_pEdict); }

	entvars_t* pev() { return &m_pEdict->v; }
	const entvars_t* pev() const { return &m_pEdict->v; }

	edict_t* edict() { return m_pEdict; }
	const edict_t* edict() const { return m_pEdict; }

	bool operator==(const EntityWrapper& rhs) const
	{
		return IsEqualTo(&rhs);
	}

	bool IsEqualTo(const EntityWrapper* pRhs) const
	{
		return (this->pev() == pRhs->pev());
	}

private:
	edict_t* m_pEdict;
};
#pragma once

#include <map>
#include <string>

#include "BaseMonster.h"

class MonsterManager
{
public:
	MonsterManager() {}
	~MonsterManager();

	BaseMonster* Create(const char* pszName);
	void Remove(BaseMonster* pMonster);
	BaseMonster* GetMonster(edict_t* pEntity) const;
	BaseMonsterClassHelper* GetMonsterClass(const char* pszName);
	void Clear();

	void Register(const char* pszName, BaseMonsterClassHelper* pHelper);

	void Precache();
	void OnThink(edict_t* pEntity);
	void OnTouch(edict_t* pEntity, edict_t* pOther);
	void OnKilled(edict_t* pVictim, edict_t* pKiller, int shouldgib);
	void OnTraceAttack(edict_t* pVictim, entvars_t*& pAttacker, float& damage, Vector& dir, TraceResult*& ptr, int& damagebits);

private:
	std::map<edict_t*, BaseMonster*> m_edicts;
	std::map<std::string, BaseMonsterClassHelper*> m_helpers;
};

MonsterManager& GetMonsterManager();
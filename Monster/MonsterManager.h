#pragma once

#include <map>
#include "extdll.h"

class MonsterBase;

class MonsterManager
{
public:
	MonsterManager() {}
	~MonsterManager();

	MonsterBase* Create();
	void Remove(MonsterBase* pMonster);

private:
	std::map<edict_t*, MonsterBase*> m_edictmap;
};
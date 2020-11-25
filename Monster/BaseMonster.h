#pragma once

#include "extdll.h"
#include "meta_api.h"

void RegisterMonsterCvars();

class BaseMonster
{
public:
	BaseMonster();
	BaseMonster(edict_t* pEntity);

	virtual ~BaseMonster();

	virtual void Precache() {}
	virtual void Spawn() {}
	virtual void Think() {}
	virtual void Touch(edict_t* pOther) {}
	virtual void Killed(edict_t* pKiller, int shouldgib) {}
	virtual void TraceAttack(entvars_t*& pAttacker, float& damage, Vector& dir, TraceResult*& ptr, int& damagebits);
	virtual void TraceBleed(float damage, Vector dir, TraceResult* ptr, int damagetype);

	virtual bool IsAlive() { return (m_pev->deadflag == DEAD_NO) && m_pev->health > 0; }
	virtual int BloodColor() { return BLOOD_COLOR_RED; }

	virtual Vector Center() { return (m_pev->absmax + m_pev->absmin) * 0.5; };
	virtual Vector EyePosition() { return m_pev->origin + m_pev->view_ofs; }
	virtual const char* GetName() { return ""; }

	edict_t* m_pEdict;
	entvars_t* m_pev;
};

#define DECLARE_MONSTER_CLASS(monster_class) \
	static __MonsterClassHelper<monster_class> __s_MonsterClassHelper(#monster_class);

class BaseMonsterClassHelper
{
public:
	BaseMonsterClassHelper(const char* pszName);

	virtual ~BaseMonsterClassHelper() = default;
	virtual BaseMonster* Instantiate() = 0;
};

template<class T>
class __MonsterClassHelper : public BaseMonsterClassHelper
{
public:
	__MonsterClassHelper<T>(const char* pszName) : BaseMonsterClassHelper(pszName) {}

	virtual BaseMonster* Instantiate() override
	{
		return new T();
	}
};
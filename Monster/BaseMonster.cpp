#include "BaseMonster.h"
#include "Base.h"
#include "MonsterManager.h"

static cvar_t monster_dmg_head_data = { "monster_dmg_head", "3.0", FCVAR_EXTDLL, 0, NULL };
static cvar_t monster_dmg_chest_data = { "monster_dmg_chest", "1.0", FCVAR_EXTDLL, 0, NULL };
static cvar_t monster_dmg_stomach_data = { "monster_dmg_stomach", "1.25", FCVAR_EXTDLL, 0, NULL };
static cvar_t monster_dmg_leg_data = { "monster_dmg_leg", "0.75", FCVAR_EXTDLL, 0, NULL };
static cvar_t monster_dmg_arm_data = { "monster_dmg_arm", "1.0", FCVAR_EXTDLL, 0, NULL };

static cvar_t* monster_dmg_head;
static cvar_t* monster_dmg_chest;
static cvar_t* monster_dmg_stomach;
static cvar_t* monster_dmg_leg;
static cvar_t* monster_dmg_arm;

BaseMonster::BaseMonster()
{
	m_pEdict = CREATE_NAMED_ENTITY(MAKE_STRING("func_wall"));
	m_pev = &m_pEdict->v;
}

BaseMonster::BaseMonster(edict_t* pEntity)
{
	m_pEdict = pEntity;
	m_pev = &pEntity->v;
}

BaseMonster::~BaseMonster()
{
	m_pev->flags |= FL_KILLME;
}

void BaseMonster::TraceAttack(entvars_t*& pAttacker, float& damage, Vector& dir, TraceResult*& ptr, int& damagetype)
{
	if (m_pev->takedamage)
	{
		switch (ptr->iHitgroup)
		{
		case HITGROUP_GENERIC:
			break;
		case HITGROUP_HEAD:
			damage *= monster_dmg_head->value;
			break;
		case HITGROUP_CHEST:
			damage *= monster_dmg_chest->value;
			break;
		case HITGROUP_STOMACH:
			damage *= monster_dmg_stomach->value;
			break;
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			damage *= monster_dmg_arm->value;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			damage *= monster_dmg_leg->value;
			break;
		default:
			break;
		}

		UTIL_BloodDrips(ptr->vecEndPos, dir, this->BloodColor(), (int)damage);
		TraceBleed(damage, dir, ptr, damagetype);
	}
}

void BaseMonster::TraceBleed(float damage, Vector dir, TraceResult* ptr, int damagetype)
{
	if (BloodColor() == DONT_BLEED)
		return;

	if (!(damagetype & (DMG_CRUSH | DMG_BULLET | DMG_SLASH | DMG_BLAST | DMG_CLUB | DMG_MORTAR)))
		return;

	// make blood decal on the wall! 
	TraceResult Bloodtr;
	Vector vecTraceDir;
	float flNoise;
	int cCount;
	int i;

	if (damage < 10)
	{
		flNoise = 0.1;
		cCount = 1;
	}
	else if (damage < 25)
	{
		flNoise = 0.2;
		cCount = 2;
	}
	else
	{
		flNoise = 0.3;
		cCount = 4;
	}

	for (i = 0; i < cCount; i++)
	{
		vecTraceDir = dir * -1;// trace in the opposite direction the shot came from (the direction the shot is going)

		vecTraceDir.x += RANDOM_FLOAT(-flNoise, flNoise);
		vecTraceDir.y += RANDOM_FLOAT(-flNoise, flNoise);
		vecTraceDir.z += RANDOM_FLOAT(-flNoise, flNoise);

		TRACE_LINE(ptr->vecEndPos, ptr->vecEndPos + vecTraceDir * -172, ignore_monsters, m_pEdict, &Bloodtr);

		if (Bloodtr.flFraction != 1.0)
		{
			UTIL_BloodDecalTrace(&Bloodtr, BloodColor());
		}
	}
}

BaseMonsterClassHelper::BaseMonsterClassHelper(const char* pszName)
{
	GetMonsterManager().Register(pszName, this);
}

void RegisterMonsterCvars()
{
	CVAR_REGISTER(&monster_dmg_head_data);
	monster_dmg_head = CVAR_GET_POINTER(monster_dmg_head_data.name);

	CVAR_REGISTER(&monster_dmg_chest_data);
	monster_dmg_chest = CVAR_GET_POINTER(monster_dmg_chest_data.name);

	CVAR_REGISTER(&monster_dmg_stomach_data);
	monster_dmg_stomach = CVAR_GET_POINTER(monster_dmg_stomach_data.name);

	CVAR_REGISTER(&monster_dmg_leg_data);
	monster_dmg_leg = CVAR_GET_POINTER(monster_dmg_leg_data.name);

	CVAR_REGISTER(&monster_dmg_head_data);
	monster_dmg_arm = CVAR_GET_POINTER(monster_dmg_arm_data.name);
}

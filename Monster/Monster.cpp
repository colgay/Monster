#include "Monster.h"
#include "Utils.h"

Monster::Monster(edict_t* pEntity) : MonsterBase(pEntity)
{
	m_pEdict->v.classname = MAKE_STRING("ai_monster");
	m_activity = ACT_RESET;
}

void Monster::Precache()
{
	PRECACHE_MODEL("models/player.mdl");
}

void Monster::Spawn(const Vector& origin, const Vector& angle)
{
	SET_ORIGIN(m_pEdict, origin);
	SET_MODEL(m_pEdict, "models/player.mdl");

	m_pEdict->v.solid = SOLID_SLIDEBOX;
	m_pEdict->v.takedamage = DAMAGE_AIM;
	m_pEdict->v.health = 100.0f;
	m_pEdict->v.max_health = m_pEdict->v.health;

	m_pEdict->v.controller[0] = 127;
	m_pEdict->v.controller[1] = 127;
	m_pEdict->v.controller[2] = 127;
	m_pEdict->v.controller[3] = 127;

	m_pEdict->v.angles = angle;

	SET_SIZE(m_pEdict, Vector(-16.0f, -16.0f, -36.0f), Vector(16.0f, 16.0f, 36.0f));

	m_pEdict->v.nextthink = gpGlobals->time + 0.1f;
}

void Monster::Think()
{


	m_pEdict->v.nextthink = gpGlobals->time + 0.1f;
}

bool Monster::IsNewPathNeeded(const Vector& target, float target_dist, float start_dist)
{
	if (!IsPathValid())
	{
		return true;
	}

	for (auto it = m_waypoints.rbegin(); it != m_waypoints.rend(); ++it)
	{

	}
}

bool Monster::IsPathValid() const
{
	for (auto it = m_waypoints.begin(); it != m_waypoints.end(); it++)
	{
		if ((*it) == nullptr)
			return false;
	}

	return true;
}

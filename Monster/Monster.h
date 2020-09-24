#pragma once

#include "MonsterBase.h"
#include "meta_api.h"
#include "WaypointApi.h"

class Node;

class Monster : public MonsterBase
{
public:
	Monster(edict_t *pEntity);

	virtual void Precache() override;
	virtual void Spawn(const Vector& origin, const Vector& angle) override;
	virtual void Think() override;

	virtual bool IsNewPathNeeded(const Vector &target, float target_dist, float start_dist);
	bool IsPathValid() const;

	Activity GetActivity() const { return m_activity; }

protected:
	Activity m_activity;
	unsigned int m_iCurrentNode;
	std::vector<Node*> m_waypoints;
};
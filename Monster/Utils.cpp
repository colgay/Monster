#include "Utils.h"
#include "meta_api.h"
#include "decals.h"

static short s_SpriteBloodSpray, s_SpriteBloodDrop;

DLL_DECALLIST s_Decals[] = {
	{ "{shot1",	0 },		// DECAL_GUNSHOT1 
	{ "{shot2",	0 },		// DECAL_GUNSHOT2
	{ "{shot3",0 },			// DECAL_GUNSHOT3
	{ "{shot4",	0 },		// DECAL_GUNSHOT4
	{ "{shot5",	0 },		// DECAL_GUNSHOT5
	{ "{lambda01", 0 },		// DECAL_LAMBDA1
	{ "{lambda02", 0 },		// DECAL_LAMBDA2
	{ "{lambda03", 0 },		// DECAL_LAMBDA3
	{ "{lambda04", 0 },		// DECAL_LAMBDA4
	{ "{lambda05", 0 },		// DECAL_LAMBDA5
	{ "{lambda06", 0 },		// DECAL_LAMBDA6
	{ "{scorch1", 0 },		// DECAL_SCORCH1
	{ "{scorch2", 0 },		// DECAL_SCORCH2
	{ "{blood1", 0 },		// DECAL_BLOOD1
	{ "{blood2", 0 },		// DECAL_BLOOD2
	{ "{blood3", 0 },		// DECAL_BLOOD3
	{ "{blood4", 0 },		// DECAL_BLOOD4
	{ "{blood5", 0 },		// DECAL_BLOOD5
	{ "{blood6", 0 },		// DECAL_BLOOD6
	{ "{yblood1", 0 },		// DECAL_YBLOOD1
	{ "{yblood2", 0 },		// DECAL_YBLOOD2
	{ "{yblood3", 0 },		// DECAL_YBLOOD3
	{ "{yblood4", 0 },		// DECAL_YBLOOD4
	{ "{yblood5", 0 },		// DECAL_YBLOOD5
	{ "{yblood6", 0 },		// DECAL_YBLOOD6
	{ "{break1", 0 },		// DECAL_GLASSBREAK1
	{ "{break2", 0 },		// DECAL_GLASSBREAK2
	{ "{break3", 0 },		// DECAL_GLASSBREAK3
	{ "{bigshot1", 0 },		// DECAL_BIGSHOT1
	{ "{bigshot2", 0 },		// DECAL_BIGSHOT2
	{ "{bigshot3", 0 },		// DECAL_BIGSHOT3
	{ "{bigshot4", 0 },		// DECAL_BIGSHOT4
	{ "{bigshot5", 0 },		// DECAL_BIGSHOT5
	{ "{spit1", 0 },		// DECAL_SPIT1
	{ "{spit2", 0 },		// DECAL_SPIT2
	{ "{bproof1", 0 },		// DECAL_BPROOF1
	{ "{gargstomp", 0 },	// DECAL_GARGSTOMP1,	// Gargantua stomp crack
	{ "{smscorch1", 0 },	// DECAL_SMALLSCORCH1,	// Small scorch mark
	{ "{smscorch2", 0 },	// DECAL_SMALLSCORCH2,	// Small scorch mark
	{ "{smscorch3", 0 },	// DECAL_SMALLSCORCH3,	// Small scorch mark
	{ "{mommablob", 0 },	// DECAL_MOMMABIRTH		// BM Birth spray
	{ "{mommablob", 0 },	// DECAL_MOMMASPLAT		// BM Mortar spray?? need decal
};

void PrecacheUtils()
{
	s_SpriteBloodSpray = PRECACHE_MODEL("sprites/bloodspray.spr");
	s_SpriteBloodDrop = PRECACHE_MODEL("sprites/blooddrop.spr");
}

char* UTIL_VarArgs(const char* format, ...)
{
	va_list argptr;
	static char string[1024];

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);

	return string;
}

Vector UTIL_GetGroundPos(const Vector &pos, int hull, float distance)
{
	TraceResult tr;
	TRACE_HULL(pos, Vector(pos.x, pos.y, pos.z - distance), ignore_monsters, hull, NULL, &tr);

	return tr.vecEndPos;
}

bool UTIL_IsVisible(const Vector& start, const Vector& end, edict_t* pEntity, int flags)
{
	TraceResult tr;
	TRACE_LINE(start, end, flags, pEntity, &tr);

	if (tr.flFraction == 1.0)
		return true;

	return false;
}

float UTIL_DistPointSegments(const Vector& p, const Vector& sp0, const Vector& sp1, Vector& out)
{
    Vector v = sp1 - sp0;
    Vector w = p - sp0;

    double c1 = DotProduct(w, v);
    if (c1 <= 0)
        return (p - sp0).Length();

    double c2 = DotProduct(v, v);
    if (c2 <= c1)
        return (p, sp1).Length();

    double b = c1 / c2;
    out = sp0 + b * v;

    return (p - out).Length();
}

void UTIL_BloodStream(const Vector& origin, const Vector& direction, int color, int amount)
{
	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, origin);
	WRITE_BYTE(TE_BLOODSTREAM);
	WRITE_COORD(origin.x);
	WRITE_COORD(origin.y);
	WRITE_COORD(origin.z);
	WRITE_COORD(direction.x);
	WRITE_COORD(direction.y);
	WRITE_COORD(direction.z);
	WRITE_BYTE(color);
	WRITE_BYTE(min(amount, 255));
	MESSAGE_END();
}

void UTIL_BloodDrips(const Vector& origin, const Vector& direction, int color, int amount)
{
	if (color == DONT_BLEED || amount == 0)
		return;

	if (amount > 255)
		amount = 255;

	amount *= 2;

	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, origin);
	WRITE_BYTE(TE_BLOODSPRITE);
	WRITE_COORD(origin.x);								// pos
	WRITE_COORD(origin.y);
	WRITE_COORD(origin.z);
	WRITE_SHORT(s_SpriteBloodSpray);				// initial sprite model
	WRITE_SHORT(s_SpriteBloodDrop);				// droplet sprite models
	WRITE_BYTE(color);								// color index into host_basepal
	WRITE_BYTE(min(max(3, amount / 10), 16));		// size
	MESSAGE_END();
}

void UTIL_BloodDecalTrace(TraceResult* pTrace, int bloodColor)
{
	if (bloodColor == BLOOD_COLOR_RED)
		UTIL_DecalTrace(pTrace, DECAL_BLOOD1 + RANDOM_LONG(0, 5));
	else
		UTIL_DecalTrace(pTrace, DECAL_YBLOOD1 + RANDOM_LONG(0, 5));
}

void UTIL_DecalTrace(TraceResult* pTrace, int decalNumber)
{
	short entityIndex;
	int index;
	int message;

	if (decalNumber < 0)
		return;

	index = s_Decals[decalNumber].index;

	if (index < 0)
		return;

	if (pTrace->flFraction == 1.0)
		return;

	// Only decal BSP models
	if (pTrace->pHit)
	{
		edict_t* pEntity = pTrace->pHit;
		if (pEntity && !(pEntity->v.solid == SOLID_BSP || pEntity->v.movetype == MOVETYPE_PUSHSTEP))
			return;

		entityIndex = ENTINDEX(pEntity);
	}
	else
		entityIndex = 0;

	message = TE_DECAL;
	if (entityIndex != 0)
	{
		if (index > 255)
		{
			message = TE_DECALHIGH;
			index -= 256;
		}
	}
	else
	{
		message = TE_WORLDDECAL;
		if (index > 255)
		{
			message = TE_WORLDDECALHIGH;
			index -= 256;
		}
	}

	MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);
	WRITE_BYTE(message);
	WRITE_COORD(pTrace->vecEndPos.x);
	WRITE_COORD(pTrace->vecEndPos.y);
	WRITE_COORD(pTrace->vecEndPos.z);
	WRITE_BYTE(index);
	if (entityIndex)
		WRITE_SHORT(entityIndex);
	MESSAGE_END();
}
#include "Utils.h"
#include "meta_api.h"

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
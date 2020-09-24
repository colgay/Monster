#pragma once

#include "extdll.h"

Vector UTIL_GetGroundPos(const Vector& pos, int hull, float distance);

bool UTIL_IsVisible(const Vector& start, const Vector& end, edict_t* pEntity, int flags);

float UTIL_DistPointSegments(const Vector& p, const Vector& sp0, const Vector& sp1, Vector &out);
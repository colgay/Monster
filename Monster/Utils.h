#pragma once

#include "extdll.h"

#define VARARGS(format, ...) UTIL_VarArgs(format, __VA_ARGS__)

char* UTIL_VarArgs(const char* format, ...);

Vector UTIL_GetGroundPos(const Vector& pos, int hull, float distance);

bool UTIL_IsVisible(const Vector& start, const Vector& end, edict_t* pEntity, int flags);

float UTIL_DistPointSegments(const Vector& p, const Vector& sp0, const Vector& sp1, Vector &out);

void UTIL_BloodStream(const Vector& origin, const Vector& direction, int color, int amount);

void UTIL_BloodDrips(const Vector& origin, const Vector& direction, int color, int amount);

void UTIL_BloodDecalTrace(TraceResult* pTrace, int bloodColor);

void UTIL_DecalTrace(TraceResult* pTrace, int decalNumber);
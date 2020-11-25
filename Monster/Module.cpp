#include "amxxmodule.h"
#include "config_parser.h"
#include "ham/hamsandwich.h"
#include "HLTypeConversion.h"
#include "MonsterManager.h"
#include "Utils.h"

static HLTypeConversion s_TypeConversion;

void OnWallTraceAttack(HamHook* hook, void* pthis, entvars_t* pattacker, float damage, Vector dir, TraceResult* ptr, int damagebits)
{
	int _state = HAM_UNSET;
	HAM_HOOK_PUSH(nullptr, nullptr, &_state);

	/* pre start */
	GetMonsterManager().OnTraceAttack(s_TypeConversion.cbase_to_edict(pthis), pattacker, damage, dir, ptr, damagebits);
	/* pre end */

	HAM_HOOK_CHECK(_state)
	{
#if defined(_WIN32)
		reinterpret_cast<void(__fastcall*)(void*, int, entvars_t*, float, Vector, TraceResult*, int)>(hook->func)(pthis, 0, pattacker, damage, dir, ptr, damagebits);
#elif defined(__linux__) || defined(__APPLE__)
		reinterpret_cast<void (*)(void*, entvars_t*, float, Vector, TraceResult*, int)>(hook->func)(pthis, pattacker, damage, dir, ptr, damagebits);
#endif

		/* post start */
		/* post end */
	}

	HAM_HOOK_POP();
}

// test return int
int OnWallTakeDamage(HamHook* hook, void* pthis, entvars_t* inflictor, entvars_t* attacker, float damage, int damagebits)
{
	int _state = HAM_UNSET;
	int _origret = 0;
	int _result = 0;
	HAM_HOOK_PUSH(&_origret, &_result, &_state); // push stack

	/* pre start */
	/* pre end */

	HAM_HOOK_CHECK(_state) // if (state < HAM_SUPERCEDE)
	{
#if defined(_WIN32)
		_origret = reinterpret_cast<int(__fastcall*)(void*, int, entvars_t*, entvars_t*, float, int)>(hook->func)(pthis, 0, inflictor, attacker, damage, damagebits);
#elif defined(__linux__) || defined(__APPLE__)
		_origret = reinterpret_cast<int (*)(void*, entvars_t*, entvars_t*, float, int)>(hook->func)(pthis, inflictor, attacker, damage, damagebits);
#endif

		/* post start */
		/* post end */
	}

	HAM_HOOK_POP(); // pop stack

	HAM_CHECK_RETURN(_state, _origret); // if not HAM_OVERRIDE, keep the original result
	return _result;
}

// test return vector
#ifdef _WIN32
void OnPlayerEyePosition(HamHook* hook, void* pthis, Vector* out)
#elif defined(__linux__) || defined(__APPLE__)
void OnPlayerEyePosition(HamHook* hook, Vector* out, void* pthis)
#endif
{
	int _state = HAM_UNSET;
	Vector _origret, _result;

	HAM_HOOK_PUSH(&_origret, &_result, &_state); // push stack

	HAM_MEMSET_VOID(&_result, 0x0, Vector);
	HAM_MEMSET_VOID(&_origret, 0x0, Vector);

	/* pre start */
	/* pre end */

	HAM_HOOK_CHECK(_state) // if (state < HAM_SUPERCEDE)
	{
#if defined(_WIN32)
		reinterpret_cast<void(__fastcall*)(void*, int, Vector*)>(hook->func)(pthis, 0, &_origret);
#elif defined(__linux__) || defined(__APPLE__)
		_origret = reinterpret_cast<Vector(*)(void*)>(hook->func)(pthis);
#endif

		/* post start */

		SET_HAM_STATE(HAM_OVERRIDE);
		SET_HAM_RESULT(Vector, Vector(369, 689, 777));

		Vector* orig = GET_HAM_ORIGRET(Vector);

		SERVER_PRINT(VARARGS("state = %d, result(%.2f, %.2f, %.2f), orig(%.2f, %.2f, %.2f)\n", 
			_state, _result.x, _result.y, _result.z, orig->x, orig->y, orig->z));

		/* post end */
	}

	HAM_HOOK_POP(); // pop stack

	HAM_CHECK_RETURN_VEC(_state, out, &_origret); // if not HAM_OVERRIDE, keep the original result
	HAM_MEMCPY_VOID(out, &_result, Vector);
	SERVER_PRINT("new result\n");
}

void OnAmxxAttach()
{
	ReadConfig();
}

void OnAmxxDetach()
{
	CloseConfigFiles();
	ClearHamHooks();
}

void ClientCommand(edict_t* pEntity)
{
	RETURN_META(MRES_IGNORED);
}

void DispatchThink(edict_t* pEntity)
{
	RETURN_META(MRES_IGNORED);
}

void OnPluginsLoaded()
{
	s_TypeConversion.init();

	SERVER_PRINT("Monster Module loaded...\n");

	RegisterHam(Ham_TraceAttack, "func_wall", reinterpret_cast<void*>(OnWallTraceAttack));
	RegisterHam(Ham_TakeDamage, "func_wall", reinterpret_cast<void*>(OnWallTakeDamage));
	RegisterHam(Ham_EyePosition, "player", reinterpret_cast<void*>(OnPlayerEyePosition));

	GetMonsterManager().Precache();
}

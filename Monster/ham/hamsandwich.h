#pragma once


#include <stack>

#include "ham_const.h"
#include "ham_hook.h"

#define HAM_HOOK_PUSH(a,b,c) GetHamHookData().push({a, b, c})
#define HAM_HOOK_STATE *GetHamHookData().top().state
#define HAM_HOOK_ORIGRET GetHamHookData().top().origret
#define HAM_HOOK_RESULT GetHamHookData().top().result

#define HAM_CHECK_RETURN_VEC(state, a, b) if (state < HAM_OVERRIDE) { HAM_MEMCPY_VOID(a, b, sizeof(Vector)); return; }

#define HAM_CHECK_RETURN(state, orig) if (state < HAM_OVERRIDE) return orig;

#define HAM_HOOK_POP() GetHamHookData().pop()

#define SET_HAM_STATE(val) if (val > HAM_HOOK_STATE) HAM_HOOK_STATE = val
#define SET_HAM_RESULT(type, val) *(reinterpret_cast<type*>(HAM_HOOK_RESULT)) = val
#define GET_HAM_ORIGRET(type) (reinterpret_cast<type*>(HAM_HOOK_ORIGRET))

#define HAM_HOOK_CHECK(state) if (state < HAM_SUPERCEDE)

#define HAM_MEMSET_VOID(a,b,c) memset(static_cast<void*>(a), b, sizeof(c))
#define HAM_MEMCPY_VOID(a,b,c) memcpy(static_cast<void*>(a), b, sizeof(c))

struct HamHookData
{
	void* origret;
	void* result;
	int* state;
};

std::stack<HamHookData>& GetHamHookData();

void* GetFunction(void* pthis, int id, bool& istramp);
void* GetHamFunction(void* pthis, int id);

int RegisterHam(int func, const char* classname, void* targetfunc);
void ClearHamHooks();
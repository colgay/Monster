// vim: set ts=4 sw=4 tw=99 noet:
//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Ham Sandwich Module
//

#ifndef HAM_HOOKLIST_T_H
#define HAM_HOOKLIST_T_H

typedef struct ham_hook_s
{
	int isset;								// whether or not this hook is registered with hamdata
	int vtid;								// vtable index of this function
	const char* name;						// name used in the keys
	bool isremoved;                         // whether or not this function is no more available in the mod
	bool isvoid;							// whether or not the target trampoline uses voids
	bool needsretbuf;						// whether or not a pointer to a memory buffer is needed to store a return value
	int  paramcount;						// how many parameters are in the func
	//void* targetfunc;						// the target hook
} ham_hook_t;

extern ham_hook_t ham_hooklist[];

#endif

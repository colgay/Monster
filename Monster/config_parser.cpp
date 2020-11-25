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

#include "config_parser.h"
#include "amxxmodule.h"
#include "ham/ham_const.h"
#include "ham/ham_hooklist.h"
#include <amtl/am-platform.h>

static IGameConfig* s_pCommonConfig;
static IGameConfigManager* s_pConfigManager;
static OffsetManager s_Offset;

IGameConfig* GetCommonConfig() { return s_pCommonConfig; }
IGameConfigManager* GetConfigManager() { return s_pConfigManager; }
OffsetManager GetOffset() { return s_Offset; }

int ReadConfig()
{
	s_pConfigManager = MF_GetConfigManager();

	char error[256] = "";

	if (!s_pConfigManager->LoadGameConfigFile("common.games", &s_pCommonConfig, error, sizeof error))
	{
		MF_Log("common.games gamedata could not be read: %s", error);
		return -1;
	}

	TypeDescription value;

	if (s_pCommonConfig->GetOffset("pev", &value))
	{
		s_Offset.SetPev(value.fieldOffset);
	}

	if (s_pCommonConfig->GetOffset("base", &value))
	{
		s_Offset.SetBase(value.fieldOffset);
	}

	for (auto index = 0; index < HAM_LAST_ENTRY_DONT_USE_ME_LOL; ++index)
	{
		if (s_pCommonConfig->GetOffset(ham_hooklist[index].name, &value))
		{
			ham_hooklist[index].isset = 1;
			ham_hooklist[index].vtid = value.fieldOffset;
		}
	}

	return 1;
}

void CloseConfigFiles()
{
	s_pConfigManager->CloseGameConfigFile(s_pCommonConfig);
}
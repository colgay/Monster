#pragma once

#include "ham/offsets.h"

IGameConfig* GetCommonConfig();
IGameConfigManager* GetConfigManager();
OffsetManager GetOffset();

int ReadConfig();
void CloseConfigFiles();
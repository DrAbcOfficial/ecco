#pragma once
#ifndef __ECCO_SCRIPT_SYS
#define __ECCO_SCRIPT_SYS
#include <vector>
#include "CEccoScriptItem.h"

extern void ResetEccoScriptItems();
extern bool LoadEccoScriptItems();

extern std::vector<CEccoScriptItem*> g_aryEccoScriptItems;

#endif // !_ECCO_SCRIPT_SYS

#pragma once
#ifndef __ECCO_SCRIPT_SYS
#define __ECCO_SCRIPT_SYS
#include <vector>
#include <extdll.h>
#include "scripts/CEccoScriptItem.h"
#include "menu/executor/CEccoScriptExecutor.h"
#include "api/IEccoScriptSystem.h"

extern void InitializeScriptSystem();

extern IEccoScriptSystem::Result EvalScriptContent(edict_t* caller, CEccoScriptExecutor* item);

extern void ResetEccoScriptItems();
extern bool LoadEccoScriptItems();
extern void PrecacheAllScriptItems();
extern IEccoScriptSystem* GetEccoScriptSystem();

extern std::vector<CEccoScriptItem*> g_aryEccoScriptItems;

#endif // !_ECCO_SCRIPT_SYS

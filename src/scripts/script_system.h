#pragma once
#ifndef __ECCO_SCRIPT_SYS
#define __ECCO_SCRIPT_SYS
#include <vector>
#include <extdll.h>
#include "scripts/CEccoScriptItem.h"

extern void InitScriptEngine();
extern bool EvalScriptContent(edict_t* caller, std::string& content);

extern void ResetEccoScriptItems();
extern bool LoadEccoScriptItems();

extern std::vector<CEccoScriptItem*> g_aryEccoScriptItems;

#endif // !_ECCO_SCRIPT_SYS

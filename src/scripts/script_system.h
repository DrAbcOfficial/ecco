#pragma once
#ifndef __ECCO_SCRIPT_SYS
#define __ECCO_SCRIPT_SYS
#include <vector>
#include <extdll.h>
#include "scripts/CEccoScriptItem.h"
#include "menu/executor/CEccoScriptExecutor.h"

extern void InitScriptEngine();

enum class EvalResult {
	Success,
	Failure,
	Error
};
extern EvalResult EvalScriptContent(edict_t* caller, CEccoScriptExecutor* item);

extern void ResetEccoScriptItems();
extern bool LoadEccoScriptItems();
extern void PrecacheAllScriptItems();

extern std::vector<CEccoScriptItem*> g_aryEccoScriptItems;

#endif // !_ECCO_SCRIPT_SYS

#pragma once
#include <extdll.h>

extern void InitHud();
extern void ResetHud();
extern void AddHudUpdateTimer(edict_t* pent);
extern void RemoveHudUpdateTimer(edict_t* pent);
#pragma once
#include "api/HookDef.h"

extern void Call_OnPlayerCreditsChanged(IPlayerStorageItem* player, int oldCredits, int& credits);
extern void Call_OnPlayerScoreToCredits(IPlayerStorageItem* player, int score, int& added);
extern void Call_OnMenuExcuted(IEccoBaseExcutor* menu, IPlayerStorageItem* player);

extern void AddHook(Hook_Type type, void* func);
extern void RemoveHook(Hook_Type type, void* func);

extern void LoadPlugins();
extern void UnloadPlugins();

extern void CallPluginQuery();
extern void CallPluginInit();
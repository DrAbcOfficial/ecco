#pragma once
#include "IPlayerStorageItem.h"
#include "IEccoBaseExcutor.h"
#include "HookDef.h"
#include <extdll.h>

class IEccoFuncs {
public:
	virtual void GetPlayerSteamId(edict_t* pent, char* dest) = 0;
	virtual edict_t* GetPlayerBySteamId(const char* steamid) = 0;
	virtual bool IsValidPlayer(edict_t* pentPlayer) = 0;
	virtual void GetGameDir(char* dest) = 0;
	virtual void FakeClientCommand(edict_t* pent, const char* cmd) = 0;
	virtual edict_t* PrivateToEdict(void* pPrivate) = 0;

	virtual IPlayerStorageItem* GetPlayerItem(int index) = 0;
	virtual IEccoBaseExcutor* GetMenuExcutor(int index) = 0;

	virtual void ClientPrintf(edict_t* target, unsigned int hud, const char* text) = 0;
	virtual void ClientPrintfAll(unsigned int hud, const char* text) = 0;

	virtual void AddHook(Hook_Type type, void* func) = 0;
	virtual void RemoveHook(Hook_Type type, void* func) = 0;

	virtual void GetTranslation(edict_t* pent, const char* code, char* dest) = 0;
};
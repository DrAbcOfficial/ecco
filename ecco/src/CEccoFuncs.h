#pragma once
#include "api/IEccoFuncs.h"
class CEccoFuncs : public IEccoFuncs{
public:
	virtual const char* GetPlayerSteamId(edict_t* pent);
	virtual edict_t* GetPlayerBySteamId(const char* steamid);
	virtual bool IsValidPlayer(edict_t* pentPlayer);
	virtual const char* GetGameDir();
	virtual void FakeClientCommand(edict_t* pent, const char* cmd);
	virtual edict_t* PrivateToEdict(void* pPrivate);

	virtual IPlayerStorageItem* GetPlayerItem(int index);
	virtual IEccoBaseExcutor* GetMenuExcutor(int index);

	virtual void ClientPrintf(edict_t* target, unsigned int hud, const char* text);
	virtual void ClientPrintfAll(unsigned int hud, const char* text);

	virtual void AddHook(Hook_Type type, void* func);
	virtual void RemoveHook(Hook_Type type, void* func);
};


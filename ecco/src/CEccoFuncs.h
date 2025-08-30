#pragma once
#include "api/IEccoFuncs.h"
class CEccoFuncs : public IEccoFuncs{
public:
	virtual void GetPlayerSteamId(edict_t* pent, char* dest);
	virtual edict_t* GetPlayerBySteamId(const char* steamid);
	virtual bool IsValidPlayer(edict_t* pentPlayer);
	virtual void GetGameDir(char* dest);
	virtual void FakeClientCommand(edict_t* pent, const char* cmd);
	virtual edict_t* PrivateToEdict(void* pPrivate);

	virtual IPlayerStorageItem* GetPlayerItem(int index);
	virtual IEccoBaseExcutor* GetMenuExcutor(int index);

	virtual void ClientPrintf(edict_t* target, unsigned int hud, const char* text);
	virtual void ClientPrintfAll(unsigned int hud, const char* text);

	virtual void AddHook(Hook_Type type, void* func);
	virtual void RemoveHook(Hook_Type type, void* func);

	virtual void GetTranslation(edict_t* pent, const char* code, char* dest);
};


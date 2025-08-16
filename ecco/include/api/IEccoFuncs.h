#pragma once
#include <extdll.h>

class IEccoFuncs {
public:
	virtual const char* GetPlayerSteamId(edict_t* pent) = 0;
	virtual edict_t* GetPlayerBySteamId(const char* steamid) = 0;
	virtual bool IsValidPlayer(edict_t* pentPlayer) = 0;
	virtual const char* GetGameDir() = 0;
	virtual void FakeClientCommand(edict_t* pent, const char* cmd) = 0;

	virtual void ClientPrintf(edict_t* target, unsigned int hud, const char* text) = 0;
	virtual void ClientPrintfAll(unsigned int hud, const char* text) = 0;
};
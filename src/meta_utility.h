#pragma once
#include <extdll.h>
#include <string>

//#define GET_PLAYER_STEAMID(pent) (*g_engfuncs.pfnGetPlayerAuthId)(pent)

//Meta mod has write float, haha
extern inline void WRITE_FLOAT(float value);
extern void WRITE_RGBA(unsigned int rgba);

extern std::string GetPlayerSteamId(edict_t* pent);
extern edict_t* GetPlayerBySteamId(const char* steamid);
extern bool IsValidPlayer(edict_t* pentPlayer);
extern std::string_view& GetGameDir();
extern void FakeClientCommand(edict_t* pent, const char* cmd);

enum class ClientPrintTarget {
	Notify = 1,
	Console = 2,
	Talk = 3,
	Center = 4
};
extern void ClientPrintf(edict_t* target, ClientPrintTarget hud, const char* text);
extern void ClientPrintfAll(ClientPrintTarget hud, const char* text);
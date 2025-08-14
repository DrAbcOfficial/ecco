#pragma once
#include <extdll.h>
#include <string>

//#define GET_PLAYER_STEAMID(pent) (*g_engfuncs.pfnGetPlayerAuthId)(pent)

extern std::string GetPlayerSteamId(edict_t* pent);
extern bool IsValidPlayer(edict_t* pentPlayer);
extern std::string_view& GetGameDir();
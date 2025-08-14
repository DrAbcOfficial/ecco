#pragma once
#include <extdll.h>
#include <string>

//#define GET_PLAYER_STEAMID(pent) (*g_engfuncs.pfnGetPlayerAuthId)(pent)

//Meta mod has write float, haha
extern inline void WRITE_FLOAT(float value);
extern void WRITE_RGBA(unsigned int rgba);

extern std::string GetPlayerSteamId(edict_t* pent);
extern bool IsValidPlayer(edict_t* pentPlayer);
extern std::string_view& GetGameDir();
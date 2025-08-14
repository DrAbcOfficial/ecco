//Copied from mmlib!

#include "meta_utility.h"
#include <meta_api.h>

void WRITE_FLOAT(float value){
	float flValue = value;
	int* intValue = reinterpret_cast<int*>(&flValue);
	WRITE_LONG(*intValue);
}

std::string GetPlayerSteamId(edict_t* pent){
	const char* infobuffer = GET_INFOKEYBUFFER(pent);
	const char* id = INFOKEY_VALUE(const_cast<char*>(infobuffer), const_cast<char*>("*sid"));
	return id;
}

bool IsValidPlayer(edict_t* pentPlayer) {
	return pentPlayer && (pentPlayer->v.flags & FL_CLIENT) != 0 && (pentPlayer->v.flags & FL_PROXY) == 0 && STRING(pentPlayer->v.netname)[0] != '\0';
}

std::string_view& GetGameDir(){
	static std::string_view dir = GET_GAME_INFO(PLID, GINFO_GAMEDIR);
	return dir;
}

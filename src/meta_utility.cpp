//Copied from mmlib!

#include "meta_utility.h"
#include <meta_api.h>

inline void WRITE_FLOAT(float value){
	WRITE_LONG(*reinterpret_cast<int*>(&value));
}

void WRITE_RGBA(unsigned int rgba){

	unsigned char r = (rgba & 0xFF000000) >> 24;
	unsigned char g = (rgba & 0xFF0000) >> 16;
	unsigned char b = (rgba & 0xFF00) >> 8;
	unsigned char a = rgba & 0xFF;
	WRITE_BYTE(r);
	WRITE_BYTE(g);
	WRITE_BYTE(b);
	WRITE_BYTE(a);
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

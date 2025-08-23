#include <vector>
#include "menu/executor/CBaseEccoExecutor.h"

#include "storage/Storage.h"

#include "CEccoFuncs.h"

#include <meta_api.h>
#include "meta_utility.h"

const char* CEccoFuncs::GetPlayerSteamId(edict_t* pent){
	static char steamid[64];
	strcpy(steamid, EccoMetaUtility::GetPlayerSteamId(pent).c_str());
	return steamid;
}

edict_t* CEccoFuncs::GetPlayerBySteamId(const char* steamid){
	return EccoMetaUtility::GetPlayerBySteamId(steamid);
}

bool CEccoFuncs::IsValidPlayer(edict_t* pentPlayer){
	return EccoMetaUtility::IsValidPlayer(pentPlayer);
}

const char* CEccoFuncs::GetGameDir(){
	return EccoMetaUtility::GetGameDir().data();
}

void CEccoFuncs::FakeClientCommand(edict_t* pent, const char* cmd){
	EccoMetaUtility::FakeClientCommand(pent, cmd);
}

edict_t* CEccoFuncs::PrivateToEdict(void* pPrivate) {
	return EccoMetaUtility::PrivateToEdict(pPrivate);
}

IPlayerStorageItem* CEccoFuncs::GetPlayerItem(int index){
	return GetPlayerStorageItem(INDEXENT(index));
}

IEccoBaseExcutor* CEccoFuncs::GetMenuExcutor(int index){
	extern std::vector<CBaseEccoExecutor*> g_aryEccoMenuExecutors;
	return g_aryEccoMenuExecutors[index];
}

void CEccoFuncs::ClientPrintf(edict_t* target, unsigned int hud, const char* text){
	EccoMetaUtility::ClientPrintf(target, static_cast<EccoMetaUtility::ClientPrintTarget>(hud), text);
}

void CEccoFuncs::ClientPrintfAll(unsigned int hud, const char* text){
	EccoMetaUtility::ClientPrintfAll(static_cast<EccoMetaUtility::ClientPrintTarget>(hud), text);
}

CEccoFuncs g_pEccoFuncs;
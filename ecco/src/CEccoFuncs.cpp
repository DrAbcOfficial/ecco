#include <vector>
#include "menu/executor/CBaseEccoExecutor.h"

#include "storage/Storage.h"
#include "plugin/plugin_system.h"
#include "lang/lang.h"

#include "CEccoFuncs.h"

#include "meta_utility.h"

void CEccoFuncs::GetPlayerSteamId(edict_t* pent, char* dest){
	const char* temp = EccoMetaUtility::GetPlayerSteamId(pent).c_str();
	strcpy(dest, temp);
}

edict_t* CEccoFuncs::GetPlayerBySteamId(const char* steamid){
	return EccoMetaUtility::GetPlayerBySteamId(steamid);
}

bool CEccoFuncs::IsValidPlayer(edict_t* pentPlayer){
	return EccoMetaUtility::IsValidPlayer(pentPlayer);
}

void CEccoFuncs::GetGameDir(char* dest){
	const char* temp = EccoMetaUtility::GetGameDir().data();
	strcpy(dest, temp);
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

void CEccoFuncs::AddHook(Hook_Type type, void* func){
	::AddHook(type, func);
}

void CEccoFuncs::RemoveHook(Hook_Type type, void* func){
	::RemoveHook(type, func);
}

void CEccoFuncs::GetTranslation(edict_t* pent, const char* code, char* dest){
	const char* temp = ::GetTranslation(pent, std::string(code)).c_str();
	strcpy(dest, temp);
}

CEccoFuncs g_pEccoFuncs;
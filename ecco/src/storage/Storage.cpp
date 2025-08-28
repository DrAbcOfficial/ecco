#include <unordered_map>
#include <algorithm>

#include <extdll.h>
#include <meta_api.h>

#include "meta_utility.h"

#include "Storage.h"
#include "config/CConfig.h"

using namespace EccoMetaUtility;

static std::unordered_map<std::string, CPlayerStorageItem*> s_mapPlayerStorage;

void RemoveAllPlayerStorage(){
	for (auto& pair : s_mapPlayerStorage) {
		delete pair.second;
	}
	s_mapPlayerStorage.clear();
}

void CleanPlayerLastCredits(edict_t* pent){
	if (pent) {
		auto item = GetPlayerStorageItem(pent);
		item->CleanLastCredits();
	}
	else {
		for (auto& pair : s_mapPlayerStorage) {
			pair.second->CleanLastCredits();
		}
	}
}

void StorageClientPutinServerHandle(edict_t* pent){
	if (!pent)
		return;
	std::string steamid = GetPlayerSteamId(pent);
	if(s_mapPlayerStorage.find(steamid) == s_mapPlayerStorage.end()){
		CPlayerStorageItem* pItem = new CPlayerStorageItem(pent);
		s_mapPlayerStorage[steamid] = pItem;
	}
}

void StorageClientDisconnectHandle(edict_t* pent){
	if (!IsValidPlayer(pent))
		return;
	std::string steamid = GetPlayerSteamId(pent);
	auto it = s_mapPlayerStorage.find(steamid);
	if (it != s_mapPlayerStorage.end()) {
		auto item = it->second;
		extern bool g_bIsSeriesMap;
		int save_set = GetEccoConfig()->StorePlayerScore;
		if (save_set < 2) {
			if (save_set <= 0)
				item->SetFlags(STORAGE_FLAGS::DELETE_WHEN_DISCONNECT, true);
			else if (!g_bIsSeriesMap)
				item->SetFlags(STORAGE_FLAGS::DELETE_WHEN_SERIES_END, true);
		}
		delete item;
		s_mapPlayerStorage.erase(it);
	}
}

CPlayerStorageItem* GetPlayerStorageItem(edict_t* pent){
	if (!IsValidPlayer(pent))
		return nullptr;
	std::string steamid = GetPlayerSteamId(pent);
	auto it = s_mapPlayerStorage.find(steamid);
	if (it != s_mapPlayerStorage.end()) {
		return it->second;
	}
	return nullptr;
}
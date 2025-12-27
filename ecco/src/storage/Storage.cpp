#include <unordered_map>
#include <algorithm>

#include <extdll.h>

#include "meta_utility.h"

#include "Storage.h"

using namespace EccoMetaUtility;

static std::unordered_map<std::string, CPlayerStorageItem*> s_mapPlayerStorage;

void StorageMapEndHandle(){
	for (auto& pair : s_mapPlayerStorage) {
		pair.second->FlagSelf();
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
	CPlayerStorageItem* pItem;
	if(s_mapPlayerStorage.find(steamid) == s_mapPlayerStorage.end()){
		pItem = new CPlayerStorageItem(pent);
		s_mapPlayerStorage[steamid] = pItem;
	}
	else
		pItem = s_mapPlayerStorage[steamid];
	pItem->FlagSelf();
}

void StorageClientDisconnectHandle(edict_t* pent){
	if (!IsValidPlayer(pent))
		return;
	std::string steamid = GetPlayerSteamId(pent);
	auto it = s_mapPlayerStorage.find(steamid);
	if (it != s_mapPlayerStorage.end()) {
		auto item = it->second;
		item->FlagSelf();
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

void StorageServerActivateHandle(){
	for (auto& pair : s_mapPlayerStorage) {
		pair.second->FlagSelf();
	}
}
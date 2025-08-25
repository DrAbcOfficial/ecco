#include <unordered_map>
#include <algorithm>

#include <extdll.h>
#include <meta_api.h>

#include "meta_utility.h"

#include "Storage.h"
#include "config/CConfig.h"

using namespace EccoMetaUtility;

static std::unordered_map<std::string, CPlayerStorageItem*> s_mapPlayerStorage;

void CleanPlayerCredites(edict_t* pent) {
	int start_money = GetEccoConfig()->PlayerStartScore;
	if (pent) {
		auto item = GetPlayerStorageItem(pent);
		item->SetCredits(start_money);
		item->SaveData();
	}
	else {
		for(auto& pair : s_mapPlayerStorage) {
			pair.second->SetCredits(start_money);
			pair.second->SaveData();
		}
	}
}

void CleanUnconnectedPlayerStorage(){
	for (auto& pair : s_mapPlayerStorage) {
		if(pair.second->m_bDeleteMe) {
			delete pair.second;
			s_mapPlayerStorage.erase(pair.first);
		}
	}
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
		it->second->m_bDeleteMe = true;
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
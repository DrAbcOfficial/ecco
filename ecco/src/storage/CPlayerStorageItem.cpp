#include <fstream>
#include <filesystem>

#include "CPlayerStorageItem.h"

#include <meta_api.h>

#include "meta_utility.h"
#include "config/CConfig.h"
#include "plugin/plugin_system.h"

#undef read
#undef close
#undef write

using namespace EccoMetaUtility;

CPlayerStorageItem::CPlayerStorageItem(edict_t* pent){
	if (pent) {
		std::string id = GetPlayerSteamId(pent);
		strcpy(m_saveData.SteamId, id.c_str());

		std::filesystem::path storage_file(GetGameDir());
		storage_file.append("addons/ecco/");
		storage_file.append(GetEccoConfig()->Path.Data);
		if(!std::filesystem::exists(storage_file))
			std::filesystem::create_directories(storage_file);
		storage_file.append(m_saveData.SteamId);
		m_szStoragePath = storage_file.string().c_str();
		if (std::filesystem::exists(m_szStoragePath))
			ReadData();
		else {
			m_saveData.Credits = 0;
			SaveData();
		}
	}
	m_iScore = 0;
	m_pPlayer = pent;
}

int CPlayerStorageItem::GetCredits() const{
	return m_saveData.Credits;
}

const char* CPlayerStorageItem::GetSteamId() const{
	return m_saveData.SteamId;
}

const char* CPlayerStorageItem::GetName() const{
	return STRING(VARS(m_pPlayer)->netname);
}

void CPlayerStorageItem::SetCredits(int ulCredits){
	Call_OnPlayerCreditsChanged(this, m_saveData.Credits, ulCredits);
	m_saveData.Credits = ulCredits;
	SaveData();
}

void CPlayerStorageItem::AddCredits(int ulCredits){
	SetCredits(m_saveData.Credits + ulCredits);
}

void CPlayerStorageItem::SaveData(){
	std::ofstream ofs(m_szStoragePath, std::ios::binary);
	ofs.write(reinterpret_cast<char*>(&m_saveData), sizeof(m_saveData));
	ofs.close();
}

void CPlayerStorageItem::ScoreToCredits(int newScore){
	if (m_iScore == newScore)
		return;
	int limit = GetEccoConfig()->MoneyLimitePerMap;
	if (limit > 0 && m_iLastCredits >= limit)
		return;
	int added = static_cast<int>((newScore - m_iScore) * GetEccoConfig()->ScoreToMoneyMultiplier);
	if(m_iLastCredits + added > limit)
		added = limit - m_iLastCredits;
	Call_OnPlayerScoreToCredits(this, newScore, added);
	AddCredits(added);
	m_iLastCredits += added;
	m_iScore = newScore;
	SaveData();
}

void CPlayerStorageItem::CleanLastCredits(){
	m_iLastCredits = 0;
}

const char* CPlayerStorageItem::GetLang(){
	return m_saveData.Lang;
}

void CPlayerStorageItem::SetLang(const char* lang){
	strncpy(m_saveData.Lang, lang, sizeof(m_saveData.Lang));
}

ADMIN_LEVEL CPlayerStorageItem::GetAdminLevel() const{
	return ADMIN_LEVEL(m_saveData.Admin);
}

void CPlayerStorageItem::SetAdminLevel(ADMIN_LEVEL level){
	m_saveData.Admin = static_cast<int>(level);
}

edict_t* CPlayerStorageItem::GetPlayer() const{
	return m_pPlayer;
}

void CPlayerStorageItem::ReadData(){
	std::ifstream ifs(m_szStoragePath, std::ios::binary);
	ifs.read(reinterpret_cast<char*>(&m_saveData), sizeof(m_saveData));
	ifs.close();
}
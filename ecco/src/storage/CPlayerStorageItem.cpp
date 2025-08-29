#include <fstream>
#include <filesystem>
#include <algorithm>

#include "CPlayerStorageItem.h"

#include <meta_api.h>

#include "meta_utility.h"
#include "config/CConfig.h"
#include "plugin/plugin_system.h"
#include "lang/lang.h"

#undef read
#undef close
#undef write

using namespace EccoMetaUtility;

CPlayerStorageItem::CPlayerStorageItem(edict_t* pent)
	: m_iScore(0), m_pPlayer(pent)
{
	if (!pent) 
		return;
	const std::string steamId = GetPlayerSteamId(pent);
	strcpy(m_saveData.SteamId, steamId.c_str());
	std::filesystem::path storagePath(GetGameDir());
	storagePath /= "addons/ecco/";
	storagePath /= GetEccoConfig()->Path.Data;
	if (!std::filesystem::exists(storagePath)) {
		std::filesystem::create_directories(storagePath);
	}
	storagePath /= m_saveData.SteamId;
	m_szStoragePath = storagePath.string().c_str();
	const auto startScore = GetEccoConfig()->PlayerStartScore;
	const bool fileExists = std::filesystem::exists(m_szStoragePath);
	if (fileExists) {
		ReadData();
		extern bool g_bIsSeriesMap;
		const bool shouldResetScore =
			(TestFlags(STORAGE_FLAGS::DELETE_WHEN_SERIES_END) && !g_bIsSeriesMap) ||
			TestFlags(STORAGE_FLAGS::DELETE_WHEN_DISCONNECT);
		if (shouldResetScore)
			SetCredits(startScore);
	}
	else {
		m_saveData.Credits = startScore;
		SaveData();
	}
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

bool CPlayerStorageItem::TestFlags(STORAGE_FLAGS flag) const{
	return (m_saveData.Flags & static_cast<unsigned long long>(flag)) != 0;
}

void CPlayerStorageItem::SetFlags(STORAGE_FLAGS flag, bool on){
	if (on)
		m_saveData.Flags |= static_cast<unsigned long long>(flag);
	else
		m_saveData.Flags &= ~static_cast<unsigned long long>(flag);
	SaveData();
}

void CPlayerStorageItem::ScoreToCredits(int newScore){
	if (m_iScore == newScore)
		return;
	int limit = GetEccoConfig()->MoneyLimitePerMap;
	if (limit > 0 && m_iLastCredits >= limit)
		return;
	int added = static_cast<int>((newScore - m_iScore) * GetEccoConfig()->ScoreToMoneyMultiplier);
	if(limit > 0 && m_iLastCredits + added > limit)
		added = limit - m_iLastCredits;
	Call_OnPlayerScoreToCredits(this, newScore, added);
	AddCredits(added);
	m_iLastCredits += added;
	m_iScore = newScore;
}

void CPlayerStorageItem::CleanLastCredits(){
	m_iLastCredits = 0;
	m_iScore = 0;
}

void CPlayerStorageItem::FlagSelf() {
	extern bool g_bIsSeriesMap;
	const int saveSet = GetEccoConfig()->StorePlayerScore;
	bool deleteOnDisconnect = false;
	bool deleteOnSeriesEnd = false;
	if (saveSet < 2) {
		if (saveSet <= 0) {
			deleteOnDisconnect = true;
		}
		else if (!g_bIsSeriesMap) {
			deleteOnSeriesEnd = true;
		}
	}
	SetFlags(STORAGE_FLAGS::DELETE_WHEN_DISCONNECT, deleteOnDisconnect);
	SetFlags(STORAGE_FLAGS::DELETE_WHEN_SERIES_END, deleteOnSeriesEnd);
}

const char* CPlayerStorageItem::GetLang(){
	return m_saveData.Lang;
}

void CPlayerStorageItem::SetLang(const char* lang){
	auto& langs = GetAvaliableLangs();
	if (std::find(langs.begin(), langs.end(), lang) == langs.end())
		strncpy(m_saveData.Lang, GetEccoConfig()->DefaultLang.c_str(), sizeof(m_saveData.Lang));
	else
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
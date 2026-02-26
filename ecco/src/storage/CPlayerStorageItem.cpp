#include <fstream>
#include <filesystem>
#include <algorithm>

#include "CPlayerStorageItem.h"
#include "ScopedFile.h"
#include "config/CConfig.h"
#include "plugin/plugin_system.h"
#include "lang/lang.h"

#include "meta_utility.h"

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
		// 检查是否应该重置分数
		// 如果有 DELETE_WHEN_SERIES_END 标志但当前不是系列图，说明系列图已结束，应该重置
		const bool shouldResetScore =
			(TestFlags(STORAGE_FLAGS::DELETE_WHEN_SERIES_END) && !g_bIsSeriesMap) ||
			(TestFlags(STORAGE_FLAGS::DELETE_WHEN_DISCONNECT));
		if (shouldResetScore) {
			SetCredits(startScore);
			// 重置分数后清除 DELETE_WHEN_SERIES_END 标志，避免重复重置
			if (TestFlags(STORAGE_FLAGS::DELETE_WHEN_SERIES_END) && !g_bIsSeriesMap) {
				SetFlags(STORAGE_FLAGS::DELETE_WHEN_SERIES_END, false);
			}
			// 如果重置了分数，也需要清除 DELETE_WHEN_DISCONNECT 标志
			if (TestFlags(STORAGE_FLAGS::DELETE_WHEN_DISCONNECT)) {
				SetFlags(STORAGE_FLAGS::DELETE_WHEN_DISCONNECT, false);
			}
		}
		FlagSelf();
		// 更新后的标志需要保存
		SaveData();
	}
	else {
		m_saveData.Credits = startScore;
		FlagSelf();
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
	MarkDirty();
}

void CPlayerStorageItem::AddCredits(int ulCredits){
	SetCredits(m_saveData.Credits + ulCredits);
}

void CPlayerStorageItem::SaveData(){
	ScopedFileWrite file(m_szStoragePath);
	if (!file.is_open()) {
		return;
	}
	file.get().write(reinterpret_cast<char*>(&m_saveData), sizeof(m_saveData));
}

bool CPlayerStorageItem::TestFlags(STORAGE_FLAGS flag) const{
	return (m_saveData.Flags & static_cast<unsigned long long>(flag)) != 0;
}

void CPlayerStorageItem::SetFlags(STORAGE_FLAGS flag, bool on){
	if (on)
		m_saveData.Flags |= static_cast<unsigned long long>(flag);
	else
		m_saveData.Flags &= ~static_cast<unsigned long long>(flag);
	MarkDirty();
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
		else {
			// 系列图保存模式
			// 如果当前是系列图，设置 DELETE_WHEN_SERIES_END 标志
			// 这样如果系列图非正常结束（崩溃、换到其他地图），下次进入时可以检测到并重置金钱
			// 如果系列图正常切换，ServerDeactivate 会保存 g_bIsSeriesMap 状态，下次进入时不会重置
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
	ScopedFileRead file(m_szStoragePath);
	if (!file.is_open()) {
		return;
	}
	file.get().read(reinterpret_cast<char*>(&m_saveData), sizeof(m_saveData));
}

void CPlayerStorageItem::FlushData(){
	if (!m_bDirty)
		return;
	SaveData();
	m_bDirty = false;
	m_fLastSaveTime = gpGlobals->time;
}

void CPlayerStorageItem::MarkDirty(){
	float saveDelay = GetEccoConfig()->SaveDelayTime;
	if (saveDelay < 0) {
		SaveData();
		return;
	}
	m_bDirty = true;
}
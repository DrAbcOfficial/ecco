#include <fstream>
#include <filesystem>

#include "CPlayerStorageItem.h"

#include <meta_api.h>

#include "meta_utility.h"
#include "CConfig.h"

#undef read
#undef close
#undef write

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
	m_pPlayer = pent;
}

int64 CPlayerStorageItem::GetCredits() const{
	return m_saveData.Credits;
}

std::string_view CPlayerStorageItem::GetSteamId() const{
	return m_saveData.SteamId;
}

void CPlayerStorageItem::SetCredits(int64 ulCredits){
	m_saveData.Credits = ulCredits;
	SaveData();
}

void CPlayerStorageItem::AddCredits(int64 ulCredits){
	m_saveData.Credits += ulCredits;
	SaveData();
}

void CPlayerStorageItem::SaveData(){
	std::ofstream ofs(m_szStoragePath, std::ios::binary);
	ofs.write(reinterpret_cast<char*>(&m_saveData), sizeof(m_saveData));
	ofs.close();
}

void CPlayerStorageItem::ReadData(){
	std::ifstream ifs(m_szStoragePath, std::ios::binary);
	ifs.read(reinterpret_cast<char*>(&m_saveData), sizeof(m_saveData));
	ifs.close();
}

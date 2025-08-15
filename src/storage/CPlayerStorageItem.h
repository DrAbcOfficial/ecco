#pragma once
#include <string>
#include <extdll.h>

enum class ADMIN_LEVEL {
	NONE,
	ADMIN,
	OWNER
};

class CPlayerStorageItem {
public:
	CPlayerStorageItem(edict_t* pent);

	int GetCredits() const;
	std::string GetSteamId() const;
	std::string GetName() const;

	void SetCredits(int ulCredits);
	void AddCredits(int ulCredits);

	void SaveData();

	void ScoreToCredits(int newScore);

	std::string GetLang();

	ADMIN_LEVEL GetAdminLevel() const;
	void SetAdminLevel(ADMIN_LEVEL level);
private:
	std::string m_szStoragePath;

	struct save_data_t
	{
		char SteamId[64]; // 0x40
		int Credits; // 0x80
		char Lang[8]; // 0x88
		short Admin;
	};
	save_data_t m_saveData{};
public:
	edict_t* m_pPlayer;
	int m_iScore;
private:
	void ReadData();
};


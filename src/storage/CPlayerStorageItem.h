#pragma once
#include <string>
#include <extdll.h>

class CPlayerStorageItem {
public:
	CPlayerStorageItem(edict_t* pent);

	int64 GetCredits() const;
	std::string_view GetSteamId() const;

	void SetCredits(int64 ulCredits);
	void AddCredits(int64 ulCredits);

	void SaveData();

	void ScoreToCredits(int newScore);

	std::string GetLang();
private:
	std::string m_szStoragePath;

	struct save_data_t
	{
		char SteamId[64]; // 0x40
		int64 Credits; // 0x80
		char Lang[8]; // 0x88
	};
	save_data_t m_saveData{};
public:
	edict_t* m_pPlayer;
	int m_iScore;
private:
	void ReadData();
};


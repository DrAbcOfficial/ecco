#pragma once
#include <string>
#include "api/IPlayerStorageItem.h"

class CPlayerStorageItem : public IPlayerStorageItem {
public:
	CPlayerStorageItem(edict_t* pent);

	virtual int GetCredits() const;
	virtual const char* GetSteamId() const;
	virtual const char* GetName() const;

	virtual void SetCredits(int ulCredits);
	virtual void AddCredits(int ulCredits);

	virtual const char* GetLang();
	virtual void SetLang(const char* lang);

	virtual ADMIN_LEVEL GetAdminLevel() const;
	virtual void SetAdminLevel(ADMIN_LEVEL level);

	virtual edict_t* GetPlayer() const;

	virtual void SaveData();

	void ScoreToCredits(int newScore);
	void CleanLastCredits();
private:
	std::string m_szStoragePath;

	struct save_data_t
	{
		char SteamId[24];
		int Credits;
		char Lang[16];
		short Admin;
	};
	save_data_t m_saveData{};
public:
	edict_t* m_pPlayer;
	int m_iScore;
	int m_iLastCredits;

	bool m_bDeleteMe = false;
private:
	void ReadData();
};


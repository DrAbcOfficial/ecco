#pragma once
#include <string>
#include "api/IPlayerStorageItem.h"

#include "angelscript/angelscript.hpp"

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

#ifdef _GAME_SVENCOOP
	void GetLangAS(CString* out);
	void SetLangAS(CString* in);
#endif

	void ScoreToCredits(int newScore);
	void CleanLastCredits();
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
	int m_iLastCredits;
private:
	void ReadData();
};


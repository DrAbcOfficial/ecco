#pragma once
#include <extdll.h>

enum class ADMIN_LEVEL {
	NONE,
	ADMIN,
	OWNER
};
class IPlayerStorageItem {
public:
	virtual int GetCredits() const = 0;
	virtual const char* GetSteamId() const = 0;
	virtual const char* GetName() const = 0;

	virtual void SetCredits(int iCredits) = 0;
	virtual void AddCredits(int iCredits) = 0;

	virtual const char* GetLang() = 0;
	virtual void SetLang(const char* lang) = 0;

	virtual ADMIN_LEVEL GetAdminLevel() const = 0;
	virtual void SetAdminLevel(ADMIN_LEVEL level) = 0;

	virtual edict_t* GetPlayer() const = 0;

	virtual void SaveData() = 0;
};

#pragma once
#include <extdll.h>

enum class ADMIN_LEVEL {
	NONE,
	ADMIN,
	OWNER
};
enum class STORAGE_FLAGS {
	NONE = 0,
	DELETE_WHEN_SERIES_END = 1 << 0,
	DELETE_WHEN_DISCONNECT = 1 << 1,
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

	virtual bool TestFlags(STORAGE_FLAGS flag) const = 0;
	virtual void SetFlags(STORAGE_FLAGS flag, bool on) = 0;

	virtual void SaveData() = 0;
};

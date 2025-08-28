#pragma once
#include "api/IEccoBaseExcutor.h"
#include <string>

class CBaseEccoExecutor : public IEccoBaseExcutor {
public:
	virtual void Excute(edict_t* pPlayer, int selection);
	virtual std::string GetDisplayName(edict_t* pPlyaer);
	virtual size_t GetIndex();
	virtual const char* GetDisplayNameRaw(edict_t* pPlyaer);
	virtual const char* GetId();
	virtual int GetCost() const;

public:
	size_t m_iIndex = 0;
	std::string m_szDisplayNameCache{};
	std::string m_szId{};
};
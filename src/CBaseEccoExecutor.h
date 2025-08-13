#pragma once
#include <string>

#include "sven_api.hpp"

class CBaseEccoExecutor {
public:
	virtual void Excute(edict_t* pPlayer, int selection);
	virtual std::string GetDisplayName(edict_t* pPlyaer);

public:
	std::string m_szId;
};
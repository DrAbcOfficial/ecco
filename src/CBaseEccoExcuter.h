#pragma once
#include <string>

#include "sven_api.hpp"

class CBaseEccoExcuter {
public:
	virtual void Excute(edict_t* pPlayer, int selection);
	virtual std::string GetDisplayName(edict_t* pPlyaer);

protected:
	std::string m_szId;
};
#pragma once
#include <bitset>

#include "menu/executor/CBaseEccoExecutor.h"

enum MenuItemFlag {
	NONE = 0,
	HIDECOST
};

class CEccoScriptExecutor : public CBaseEccoExecutor {
public:
	virtual void Excute(edict_t* pPlayer, int selection) override;

	virtual std::string GetDisplayName(edict_t* pPlyaer) override;
public:
	std::bitset<32> m_bitFlags;
	std::string m_szScript;
	int m_iCost;
	CBaseEccoExecutor* m_pParent = nullptr; // parent menu, if any
};
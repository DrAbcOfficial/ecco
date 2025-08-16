#pragma once
#include "menu/executor/CBaseEccoExecutor.h"
class CEccoBackExecutor : public CBaseEccoExecutor
{
public:
	virtual void Excute(edict_t* pPlayer, int selection) override;
	CBaseEccoExecutor* m_pParentExecutor;
};


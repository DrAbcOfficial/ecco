#pragma once
#include "CBaseEccoExcuter.h"
class CEccoExitItem : public CBaseEccoExcuter{
	CEccoExitItem();
	virtual void Excute(edict_t* pPlayer, int selection) override;
};


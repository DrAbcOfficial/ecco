#pragma once
#include "IEccoExcuter.h"
class CEccoMenuTerminal : public IEccoExcuter{
public:
	virtual void Excute(CBasePlayer* pPlayer, int8_t iPage, bool bReOpen);

	int m_iCost;
	std::string m_szScripts;
};


#pragma once
#include "sven_api.hpp"

class IEccoExcuter {
public:
	virtual void Excute(edict_t* pPlayer, int8_t iPage, bool bReOpen) = 0;
};
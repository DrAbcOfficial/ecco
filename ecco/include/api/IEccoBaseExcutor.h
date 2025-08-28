#pragma once
#include <extdll.h>

class IEccoBaseExcutor {
public:
	virtual void Excute(edict_t* pPlayer, int selection) = 0;
	virtual const char* GetDisplayNameRaw(edict_t* pPlyaer) = 0;
	virtual const char* GetId() = 0;
	virtual size_t GetIndex() = 0;
	virtual int GetCost() const = 0;
};
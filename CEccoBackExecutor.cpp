#include "CEccoBackExecutor.h"
void CEccoBackExecutor::Excute(edict_t* pPlayer, int selection){
	if(m_pParentExecutor)
		m_pParentExecutor->Excute(pPlayer, selection);
}
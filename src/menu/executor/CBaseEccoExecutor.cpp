#include "lang/lang.h"

#include "CBaseEccoExecutor.h"

void CBaseEccoExecutor::Excute(edict_t* pPlayer, int selection) {

}

std::string CBaseEccoExecutor::GetDisplayName(edict_t* pPlyaer){
	return GetTranslation(pPlyaer, m_szId);
}

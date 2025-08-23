#include "lang/lang.h"

#include "CBaseEccoExecutor.h"
#include "storage/Storage.h"
#include "plugin/plugin_system.h"

void CBaseEccoExecutor::Excute(edict_t* pPlayer, int selection) {
	Call_OnMenuExcuted(this, GetPlayerStorageItem(pPlayer));
}

std::string CBaseEccoExecutor::GetDisplayName(edict_t* pPlyaer){
	return GetTranslation(pPlyaer, m_szId);
}

size_t CBaseEccoExecutor::GetIndex(){
	return m_iIndex;
}

const char* CBaseEccoExecutor::GetDisplayNameRaw(edict_t* pPlyaer) {
    m_szDisplayNameCache = GetDisplayName(pPlyaer);
    return m_szDisplayNameCache.c_str();
}

const char* CBaseEccoExecutor::GetId(){
	return m_szId.c_str();
}

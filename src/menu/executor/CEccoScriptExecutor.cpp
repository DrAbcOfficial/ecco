#include "storage/Storage.h"
#include "scripts/script_system.h"

#include "CEccoScriptExecutor.h"

void CEccoScriptExecutor::Excute(edict_t* pPlayer, int selection){
    auto storage = GetPlayerStorageItem(pPlayer);
    int remain = storage->GetCredits();
    if(remain < m_iCost){
        return;
	}
	storage->AddCredits((int64) - m_iCost);
	EvalScriptContent(pPlayer, m_szScript);
}

std::string CEccoScriptExecutor::GetDisplayName(edict_t* pPlyaer){
    if (m_bitFlags.test(HIDECOST))
        return m_szId;
    return m_szId + " - " + std::to_string(m_iCost);
}

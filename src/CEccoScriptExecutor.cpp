#include "CEccoScriptExecutor.h"

void CEccoScriptExecutor::Excute(edict_t* pPlayer, int selection){
}

std::string CEccoScriptExecutor::GetDisplayName(edict_t* pPlyaer){
    if (m_bitFlags.test(HIDECOST))
        return m_szId;
    return m_szId + " - " + std::to_string(m_iCost);
}

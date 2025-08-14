#include "storage/Storage.h"
#include "scripts/script_system.h"
#include "lang/lang.h"

#include "CEccoScriptExecutor.h"

void CEccoScriptExecutor::Excute(edict_t* pPlayer, int selection){
    auto storage = GetPlayerStorageItem(pPlayer);
    int64 remain = storage->GetCredits();
    if(remain < (int64)m_iCost){
		ClientPrintfTranslation(pPlayer, ClientPrintTarget::Talk, "not_enough_credits", 
            StringToU8String(GetTranslation(pPlayer, m_szId)), StringToU8String(std::to_string(m_iCost)), StringToU8String(std::to_string(remain)));
        return;
	}
	storage->AddCredits(-m_iCost);
	EvalScriptContent(pPlayer, m_szScript);
}

std::string CEccoScriptExecutor::GetDisplayName(edict_t* pPlyaer){
    if (m_bitFlags.test(HIDECOST))
        return GetTranslation(pPlyaer, m_szId);
    return  GetTranslation(pPlyaer, m_szId) + " - " + std::to_string(m_iCost);
}

#include "storage/Storage.h"
#include "scripts/script_system.h"
#include "lang/lang.h"
#include "config/CConfig.h"

#include "CEccoScriptExecutor.h"

using namespace EccoMetaUtility;

void CEccoScriptExecutor::Excute(edict_t* pPlayer, int selection){
    if (GetEccoConfig()->BuyMenu.ReOpenMenuAfterBuy)
        m_pParent->Excute(pPlayer, selection);

    auto storage = GetPlayerStorageItem(pPlayer);
    int remain = storage->GetCredits();
    if(remain < m_iCost){
		ClientPrintfTranslation(pPlayer, ClientPrintTarget::Talk, "not_enough_credits", 
            StringToU8String(GetTranslation(pPlayer, m_szId)), StringToU8String(std::to_string(m_iCost)), StringToU8String(std::to_string(remain)));
        CBaseEccoExecutor::Excute(pPlayer, selection);
        return;
	}
    IEccoScriptSystem::Result ret = EvalScriptContent(pPlayer, this);
    switch (ret) {
    case IEccoScriptSystem::Result::OK: storage->AddCredits(-m_iCost); break;
    case IEccoScriptSystem::Result::Error: ClientPrintfTranslation(pPlayer, ClientPrintTarget::Talk, "ecco_eval_exception", StringToU8String(GetTranslation(pPlayer, m_szId)));break;
    default: break;
    }
    CBaseEccoExecutor::Excute(pPlayer, selection);
}

std::string CEccoScriptExecutor::GetDisplayName(edict_t* pPlyaer){
    if (m_bitFlags.test(HIDECOST))
        return GetTranslation(pPlyaer, m_szId);
    return  GetTranslation(pPlyaer, m_szId) + " - " + std::to_string(m_iCost);
}

int CEccoScriptExecutor::GetCost() const{
    return m_iCost;
}

std::string CEccoScriptExecutor::GetDisplayNameFromStorage(CPlayerStorageItem* chaiPlayer){
    return GetDisplayName(chaiPlayer->m_pPlayer);
}

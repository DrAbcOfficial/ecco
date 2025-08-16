#include "basescriptmethods.h"

#include <meta_api.h>
#include "meta_utility.h"

static edict_t* GiveNamedItem_Common(edict_t* pev, const char* pszName){
	int istr = MAKE_STRING(pszName);
	edict_t* pent = CREATE_NAMED_ENTITY(istr);
	if (FNullEnt(pent)){
		ALERT(at_console, "NULL Ent in GiveNamedItem!\n");
		return nullptr;
	}
	VARS(pent)->origin = VARS(pev)->origin;
	// SF_NORESPAWN 
	pent->v.spawnflags |= (1 << 30);
	MDLL_Spawn(pent);
	return pent;
}

void EccoBaseScriptMethods::give(CPlayerStorageItem* player, std::string name){
	edict_t* pev = player->m_pPlayer;
	edict_t* pent = GiveNamedItem_Common(pev, name.c_str());
	if (!pent)
		return;
	MDLL_Touch(pent, pev);
}

void EccoBaseScriptMethods::saytext(CPlayerStorageItem* player, std::string content){
	ClientPrintf(player->m_pPlayer, ClientPrintTarget::Talk, content.c_str());
}

void EccoBaseScriptMethods::saytextall(std::string content){
	ClientPrintfAll(ClientPrintTarget::Talk, content.c_str());
}

void EccoBaseScriptMethods::addmoney(CPlayerStorageItem* player, int money) {
	player->AddCredits(money);
}
void EccoBaseScriptMethods::setmoney(CPlayerStorageItem* player, int money){
	player->SetCredits(money);
}
int EccoBaseScriptMethods::getmoney(CPlayerStorageItem* player){
	return player->GetCredits();
}
void EccoBaseScriptMethods::maxhealth(CPlayerStorageItem* player, int value){
	player->m_pPlayer->v.max_health = static_cast<float>(value);
}

void EccoBaseScriptMethods::maxarmor(CPlayerStorageItem* player, int value){
	player->m_pPlayer->v.armortype = static_cast<float>(value);
}

void EccoBaseScriptMethods::maxspeed(CPlayerStorageItem* player, int value){
	player->m_pPlayer->v.maxspeed = static_cast<float>(value);
}

void EccoBaseScriptMethods::gravity(CPlayerStorageItem* player, int value){
	player->m_pPlayer->v.gravity = (value);
}

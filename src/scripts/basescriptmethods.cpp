#include "basescriptmethods.h"
#include <meta_api.h>

static edict_t* GiveNamedItem_Common(edict_t* pev, const char* pszName)
{
	int istr = MAKE_STRING(pszName);

	edict_t* pent = CREATE_NAMED_ENTITY(istr);
	if (FNullEnt(pent))
	{
		ALERT(at_console, "NULL Ent in GiveNamedItem!\n");
		return nullptr;
	}
	VARS(pent)->origin = VARS(pev)->origin;
	// SF_NORESPAWN 
	pent->v.spawnflags |= (1 << 30);
	MDLL_Spawn(pent);
	return pent;
}

void EccoBaseScriptMethods::give(CChaiPlayer* player, std::string name){
	edict_t* pev = (edict_t*)player->m_pEdict;
	edict_t* pent = GiveNamedItem_Common(pev, name.c_str());
	if (!pent)
		return;
	MDLL_Touch(pent, pev);
}
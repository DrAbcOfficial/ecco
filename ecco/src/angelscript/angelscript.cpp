#ifdef _GAME_SVENCOOP
#include <extdll.h>
#include <meta_api.h>
#include "meta_utility.h"

#include "storage/Storage.h"

#include "angelscript/angelscript.hpp"

angelhook_t g_AngelHook;

static CPlayerStorageItem* SC_SERVER_DECL CASPlayerFuncs_GetEccoStorage(void* pthis, SC_SERVER_DUMMYARG void* player) {
	edict_t* plr = EccoMetaUtility::PrivateToEdict(player);
	if (FNullEnt(plr))
		return nullptr;
	return GetPlayerStorageItem(plr);
}

void RegisterAngelScriptMethods(){
	ASEXT_RegisterDocInitCallback([](CASDocumentation* pASDoc) {
		asSFuncPtr reg;
		ASEXT_RegisterObjectType(pASDoc, "Ecco player storage", "CEccoPlayerStorage", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player credits", "CEccoPlayerStorage", "int GetCredits()", CPlayerStorageItem, GetCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player credits", "CEccoPlayerStorage", "void SetCredits(int credits)", CPlayerStorageItem, SetCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Add player credits", "CEccoPlayerStorage", "void AddCredits(int credits)", CPlayerStorageItem, AddCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player language", "CEccoPlayerStorage", "void GetLang(string&out lang)", CPlayerStorageItem, GetLangAS, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player language", "CEccoPlayerStorage", "void SetLangAS(string&in lang)", CPlayerStorageItem, SetLangAS, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player ecco admin level", "CEccoPlayerStorage", "int GetAdminLevel()", CPlayerStorageItem, GetAdminLevel, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player ecco admin level", "CEccoPlayerStorage", "void SetAdminLevel(int level)", CPlayerStorageItem, SetAdminLevel, asCALL_THISCALL);
	
		ASEXT_RegisterObjectMethod(pASDoc,
			"Get Ecco storage", "CPlayerFuncs", "CEccoPlayerStorage@ GetEccoStorage(CBasePlayer@ pPlayer)",
			(void*)CASPlayerFuncs_GetEccoStorage, asCALL_THISCALL);
	});
}

void RegisterAngelScriptHooks(){

}
#endif // _GAME_SVENCOOP
#include <extdll.h>
#include <meta_api.h>
#include "angelscript.hpp"
#include "api/IEccoPlugin.h"

extern IEccoFuncs* g_pEccoFuncs;

angelhook_t g_AngelHook;

class IASPlayerStorageItem : public IPlayerStorageItem {
	public:
	void GetLangAS(CString* lang) {
		const char* l = GetLang();
		lang->assign(l, strlen(l));
	}
	void SetLangAS(CString* lang) {
		SetLang(lang->c_str());
	}
};

static IPlayerStorageItem* SC_SERVER_DECL CASPlayerFuncs_GetEccoStorage(void* pthis, SC_SERVER_DUMMYARG void* player) {
	edict_t* plr = g_pEccoFuncs->PrivateToEdict(player);
	if (FNullEnt(plr))
		return nullptr;
	int index = ENTINDEX(plr);
	return g_pEccoFuncs->GetPlayerItem(index);
}

void RegisterAngelScriptMethods(){
	ASEXT_RegisterDocInitCallback([](CASDocumentation* pASDoc) {
		asSFuncPtr reg;
		ASEXT_RegisterObjectType(pASDoc, "Ecco player storage", "CEccoPlayerStorage", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player credits", "CEccoPlayerStorage", "int GetCredits()", IASPlayerStorageItem, GetCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player credits", "CEccoPlayerStorage", "void SetCredits(int credits)", IASPlayerStorageItem, SetCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Add player credits", "CEccoPlayerStorage", "void AddCredits(int credits)", IASPlayerStorageItem, AddCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player language", "CEccoPlayerStorage", "void GetLang(string&out lang)", IASPlayerStorageItem, GetLangAS, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player language", "CEccoPlayerStorage", "void SetLangAS(string&in lang)", IASPlayerStorageItem, SetLangAS, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player ecco admin level", "CEccoPlayerStorage", "int GetAdminLevel()", IASPlayerStorageItem, GetAdminLevel, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player ecco admin level", "CEccoPlayerStorage", "void SetAdminLevel(int level)", IASPlayerStorageItem, SetAdminLevel, asCALL_THISCALL);
	
		ASEXT_RegisterObjectMethod(pASDoc,
			"Get Ecco storage", "CPlayerFuncs", "CEccoPlayerStorage@ GetEccoStorage(CBasePlayer@ pPlayer)",
			(void*)CASPlayerFuncs_GetEccoStorage, asCALL_THISCALL);
	});
}

void RegisterAngelScriptHooks(){

}
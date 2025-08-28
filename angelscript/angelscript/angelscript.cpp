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

class IASEccoBaseExcutor : public IEccoBaseExcutor {
public:
	void ASExcute(void* pPlayer, int selection) {
		edict_t* plr = g_pEccoFuncs->PrivateToEdict(pPlayer);
		if (FNullEnt(plr))
			return;
		Excute(plr, selection);
	}
	void ASGetDisplayNameRaw(CString* str, void* pPlyaer) {
		edict_t* plr = g_pEccoFuncs->PrivateToEdict(pPlyaer);
		if (FNullEnt(plr))
			return;
		const char* name = GetDisplayNameRaw(plr);
		str->assign(name, strlen(name));
	}
	void ASGetId(CString* str) {
		const char* id = GetId();
		str->assign(id, strlen(id));
	}
};

class CEccoFunc{
public:
	IPlayerStorageItem* GetPlayerStorage(int index){
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent))
			return nullptr;
		return g_pEccoFuncs->GetPlayerItem(index);
	}
	IPlayerStorageItem* GetPlayerStoragePrivate(void* player) {
		edict_t* plr = g_pEccoFuncs->PrivateToEdict(player);
		if (FNullEnt(plr))
			return nullptr;
		int index = ENTINDEX(plr);
		return g_pEccoFuncs->GetPlayerItem(index);
	}
	IASEccoBaseExcutor* GetMenuByIndex(int index) {
		auto menu = g_pEccoFuncs->GetMenuExcutor(index);
		return static_cast<IASEccoBaseExcutor*>(menu);
	}
};
CEccoFunc g_EccoFunc;

static IPlayerStorageItem* SC_SERVER_DECL CASPlayerFuncs_GetEccoStorage(void* pthis, SC_SERVER_DUMMYARG void* player) {
	
}

void RegisterAngelScriptMethods(){
	ASEXT_RegisterDocInitCallback([](CASDocumentation* pASDoc) {
		asSFuncPtr reg;
		ASEXT_RegisterObjectType(pASDoc, "Ecco player storage", "CEccoPlayerStorage", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player credits", "CEccoPlayerStorage", "int GetCredits()", IASPlayerStorageItem, GetCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player credits", "CEccoPlayerStorage", "void SetCredits(int credits)", IASPlayerStorageItem, SetCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Add player credits", "CEccoPlayerStorage", "void AddCredits(int credits)", IASPlayerStorageItem, AddCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player language", "CEccoPlayerStorage", "void GetLang(string&out lang)", IASPlayerStorageItem, GetLangAS, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player language", "CEccoPlayerStorage", "void SetLang(string&in lang)", IASPlayerStorageItem, SetLangAS, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player ecco admin level", "CEccoPlayerStorage", "int GetAdminLevel()", IASPlayerStorageItem, GetAdminLevel, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player ecco admin level", "CEccoPlayerStorage", "void SetAdminLevel(int level)", IASPlayerStorageItem, SetAdminLevel, asCALL_THISCALL);


		ASEXT_RegisterObjectType(pASDoc, "Ecco menu excutor", "CEccoMenuExcutor", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Excute a ecco menu", "CEccoMenuExcutor", "void Excute(CBasePlayer@ pPlayer, int selection)", IASEccoBaseExcutor, ASExcute, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get display name to a player", "CEccoMenuExcutor", "void GetDisplayName(string& out display, CBasePlayer@ pPlayer)", IASEccoBaseExcutor, ASGetDisplayNameRaw, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Id", "CEccoMenuExcutor", "void GetId(string& out id)", IASEccoBaseExcutor, ASGetId, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Index", "CEccoMenuExcutor", "uint GetIndex()", IASEccoBaseExcutor, GetIndex, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Cost", "CEccoMenuExcutor", "int GetCost()", IASEccoBaseExcutor, GetCost, asCALL_THISCALL);


		ASEXT_RegisterObjectType(pASDoc, "Ecco functions", "CEccoFuncs", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco storage", "CEccoFuncs", "CEccoPlayerStorage@ GetEccoStorage(CBasePlayer@ pPlayer)", CEccoFunc, GetPlayerStoragePrivate, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco storage", "CEccoFuncs", "CEccoPlayerStorage@ GetEccoStorage(int player_index)", CEccoFunc, GetPlayerStorage, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco menu", "CEccoFuncs", "CEccoMenuExcutor@ GetMenuByIndex(int menu_index)", CEccoFunc, GetMenuByIndex, asCALL_THISCALL);

		ASEXT_RegisterGlobalProperty(pASDoc, "Ecco functions", "CEccoFuncs@ g_EccoFuncs", &g_EccoFunc);
	});
}

void RegisterAngelScriptHooks(){
	AS_CREATE_HOOK(PlayerCreditsChanged, "call before player credits changed", 
		"Player", "PlayerCreditsChanged", "CBasePlayer@ pPlayer, int oldCredits, int newCredits, int& out outCredits");
	AS_CREATE_HOOK(MenuExcuted, "call after menu excuted",
		"Menu", "MenuExcuted", "CEccoMenuExcutor@ pMenu, CBasePlayer@ pPlayer");
	AS_CREATE_HOOK(PlayerScoreToCredits, "call before player score to credits",
		"Player", "PlayerScoreToCredits", "CBasePlayer@ pPlayer, int score, int added, int& out newadded");
}
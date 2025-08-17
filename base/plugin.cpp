#include "plugin.h"

//Ecco
IEccoFuncs* g_pEccoFuncs;
IEccoScriptSystem* g_pScriptSystem;
//HLSDK
enginefuncs_t g_engfuncs;
globalvars_t* gpGlobals;
//MetaAPI
meta_globals_t* gpMetaGlobals;		// metamod globals
gamedll_funcs_t* gpGamedllFuncs;	// gameDLL function tables
mutil_funcs_t* gpMetaUtilFuncs;		// metamod utility functions

static edict_t* GiveNamedItem_Common(edict_t* pev, const char* pszName) {
	int istr = MAKE_STRING(pszName);
	edict_t* pent = CREATE_NAMED_ENTITY(istr);
	if (FNullEnt(pent)) {
		ALERT(at_console, "NULL Ent in GiveNamedItem!\n");
		return nullptr;
	}
	VARS(pent)->origin = VARS(pev)->origin;
	// SF_NORESPAWN 
	pent->v.spawnflags |= (1 << 30);
	MDLL_Spawn(pent);
	return pent;
}
static bool give(IPlayerStorageItem* player, const char* name) {
	edict_t* pev = player->GetPlayer();
	edict_t* pent = GiveNamedItem_Common(pev, name);
	if (!pent)
		return false;
	MDLL_Touch(pent, pev);
	return true;
}
static bool saytext(IPlayerStorageItem* player, const char* content) {
	g_pEccoFuncs->ClientPrintf(player->GetPlayer(), 3, content);
	return true;
}
static bool saytextall(const char* content) {
	g_pEccoFuncs->ClientPrintfAll(3, content);
	return true;
}
static bool addmoney(IPlayerStorageItem* player, int money) {
	player->AddCredits(money);
	return true;
}
static bool setmoney(IPlayerStorageItem* player, int money) {
	player->SetCredits(money);
	return true;
}
static int getmoney(IPlayerStorageItem* player) {
	return player->GetCredits();
}
static bool maxhealth(IPlayerStorageItem* player, int value) {
	player->GetPlayer()->v.max_health = static_cast<float>(value);
	return true;
}
static bool maxarmor(IPlayerStorageItem* player, int value) {
	player->GetPlayer()->v.armortype = static_cast<float>(value);
	return true;
}
static bool maxspeed(IPlayerStorageItem* player, int value) {
	player->GetPlayer()->v.maxspeed = static_cast<float>(value);
	return true;
}
static bool gravity(IPlayerStorageItem* player, int value) {
	player->GetPlayer()->v.gravity = static_cast<float>(value);
	return true;
}

class CEccoPlugin : public IEccoPlugin {
	virtual const char* GetName() const { return "Base"; };
	virtual const char* GetVersion() const { return "1.0"; };
	virtual const char* GetAuthor() const { return "Dr.Abc"; };

	void RegisteMethods() {
		g_pScriptSystem->AddMethod("give", &give);
		g_pScriptSystem->AddMethod("saytext", &saytext);
		g_pScriptSystem->AddMethod("saytextall", &saytextall);
		g_pScriptSystem->AddMethod("addmoney", &addmoney);
		g_pScriptSystem->AddMethod("getmoney", &getmoney);
		g_pScriptSystem->AddMethod("maxhealth", &maxhealth);
		g_pScriptSystem->AddMethod("maxarmor", &maxarmor);
		g_pScriptSystem->AddMethod("maxspeed", &maxspeed);
		g_pScriptSystem->AddMethod("setmoney", &setmoney);
		g_pScriptSystem->AddMethod("gravity", &gravity);
	}

	virtual void Initialize(IEccoScriptSystem* script_system, IEccoFuncs* ecco_funcs,
		enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals,
		meta_globals_t* penMetaGlobals, gamedll_funcs_t* penGamedllFuncs, mutil_funcs_t* penMetaUtilFuncs) {
		g_pEccoFuncs = ecco_funcs;
		g_pScriptSystem = script_system;
		memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
		gpGlobals = pGlobals;
		gpMetaGlobals = penMetaGlobals;
		gpGamedllFuncs = penGamedllFuncs;
		gpMetaUtilFuncs = penMetaUtilFuncs;

		RegisteMethods();
	};

	virtual void Shutdown() {};
};
static CEccoPlugin s_EccoPluginInstance;

ECCO_API IEccoPlugin* GetEccoPluginInstance() {
	return &s_EccoPluginInstance;
}
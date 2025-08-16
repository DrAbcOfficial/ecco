#include <string>
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
static void give(IPlayerStorageItem* player, std::string name) {
	edict_t* pev = player->GetPlayer();
	edict_t* pent = GiveNamedItem_Common(pev, name.c_str());
	if (!pent)
		return;
	MDLL_Touch(pent, pev);
}
static void saytext(IPlayerStorageItem* player, std::string content) {
	g_pEccoFuncs->ClientPrintf(player->GetPlayer(), 3, content.c_str());
}
static void saytextall(std::string content) {
	g_pEccoFuncs->ClientPrintfAll(3, content.c_str());
}
static void addmoney(IPlayerStorageItem* player, int money) {
	player->AddCredits(money);
}
static void setmoney(IPlayerStorageItem* player, int money) {
	player->SetCredits(money);
}
static int getmoney(IPlayerStorageItem* player) {
	return player->GetCredits();
}
static void maxhealth(IPlayerStorageItem* player, int value) {
	player->GetPlayer()->v.max_health = static_cast<float>(value);
}
static void maxarmor(IPlayerStorageItem* player, int value) {
	player->GetPlayer()->v.armortype = static_cast<float>(value);
}
static void maxspeed(IPlayerStorageItem* player, int value) {
	player->GetPlayer()->v.maxspeed = static_cast<float>(value);
}
static void gravity(IPlayerStorageItem* player, int value) {
	player->GetPlayer()->v.gravity = static_cast<float>(value);
}

class CEccoPlugin : public IEccoPlugin {
	virtual const char* GetName() const { return "Base"; };
	virtual const char* GetVersion() const { return "1.0"; };
	virtual const char* GetAuthor() const { return "Dr.Abc"; };

	void RegisteMethods() {
		auto chai_engine = g_pScriptSystem->GetScriptEngine();
		chai_engine->add(chaiscript::fun(&give), "give");
		chai_engine->add(chaiscript::fun(&saytext), "saytext");
		chai_engine->add(chaiscript::fun(&saytextall), "saytextall");
		chai_engine->add(chaiscript::fun(&addmoney), "addmoney");
		chai_engine->add(chaiscript::fun(&setmoney), "setmoney");
		chai_engine->add(chaiscript::fun(&getmoney), "getmoney");
		chai_engine->add(chaiscript::fun(&maxhealth), "maxhealth");
		chai_engine->add(chaiscript::fun(&maxarmor), "maxarmor");
		chai_engine->add(chaiscript::fun(&maxspeed), "maxspeed");
		chai_engine->add(chaiscript::fun(&gravity), "gravity");
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
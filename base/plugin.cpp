#include "sdk_api.h"
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
static bool console(const char* content) {
	ALERT(ALERT_TYPE::at_console, content);
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
static bool heal(IPlayerStorageItem* player, int value) {
	CBasePlayer* pPlayer = static_cast<CBasePlayer*>(player->GetPlayer()->pvPrivateData);
	if (!pPlayer)
		return false;
	pPlayer->TakeHealth(static_cast<float>(value), DMG_MEDKITHEAL, 0);
	return true;
}
static bool armor(IPlayerStorageItem* player, int value) {
	CBasePlayer* pPlayer = static_cast<CBasePlayer*>(player->GetPlayer()->pvPrivateData);
	if (!pPlayer)
		return false;
	pPlayer->TakeArmor(static_cast<float>(value), DMG_MEDKITHEAL, 0);
	return true;
}
static bool hurt(IPlayerStorageItem* player, int value) {
	CBasePlayer* pPlayer = static_cast<CBasePlayer*>(player->GetPlayer()->pvPrivateData);
	if (!pPlayer)
		return false;
	pPlayer->TakeDamage(nullptr, nullptr, static_cast<float>(value), DMG_GENERIC);
	return true;
}

static int create_entity(const char* pszName) {
	int istr = MAKE_STRING(pszName);
	edict_t* pent = CREATE_NAMED_ENTITY(istr);
	if (FNullEnt(pent)) {
		ALERT(at_console, "NULL Ent in CreateNamedEntity!\n");
		return -1;
	}
	return ENTINDEX(pent);
}
static bool spawn_entity(int index) {
	if (index < 0)
		return false;
	auto pent = INDEXENT(index);
	if (FNullEnt(pent)) {
		ALERT(at_console, "Try to Spawn a NULL ent!\n");
		return false;
	}
	MDLL_Spawn(pent);
	return true;
}
static bool get_player_origin(IPlayerStorageItem* player, float* x, float* y, float* z) {
	auto pent = player->GetPlayer();
	if (FNullEnt(pent)) {
		ALERT(at_console, "Try to get origin a NULL player!\n");
		return false;
	}
	*x = pent->v.origin[0];
	*y = pent->v.origin[1];
	*z = pent->v.origin[2];
	return true;
}
static bool get_entity_origin(int index, float* x, float* y, float* z) {
	auto pent = INDEXENT(index);
	if (FNullEnt(pent)) {
		ALERT(at_console, "Try to get origin a NULL ent!\n");
		return false;
	}
	*x = pent->v.origin[0];
	*y = pent->v.origin[1];
	*z = pent->v.origin[2];
	return true;
}

#define PLUGIN_VERSION __DATE__ "T" __TIME__

class CEccoPlugin : public IEccoPlugin {
	virtual const char* GetName() const { return "Base"; };
	virtual const char* GetDescription() const { return "Core"; }
	virtual const char* GetVersion() const { return PLUGIN_VERSION; };
	virtual const char* GetAuthor() const { return "Dr.Abc"; };

	void RegisteMethods() {
		g_pScriptSystem->AddMethod("give", &give);
		g_pScriptSystem->AddMethod("saytext", &saytext);
		g_pScriptSystem->AddMethod("saytextall", &saytextall);
		g_pScriptSystem->AddMethod("console", &console);
		g_pScriptSystem->AddMethod("addmoney", &addmoney);
		g_pScriptSystem->AddMethod("getmoney", &getmoney);
		g_pScriptSystem->AddMethod("maxhealth", &maxhealth);
		g_pScriptSystem->AddMethod("maxarmor", &maxarmor);
		g_pScriptSystem->AddMethod("maxspeed", &maxspeed);
		g_pScriptSystem->AddMethod("setmoney", &setmoney);
		g_pScriptSystem->AddMethod("gravity", &gravity);
		g_pScriptSystem->AddMethod("heal", &heal);
		g_pScriptSystem->AddMethod("armor", &armor);
		g_pScriptSystem->AddMethod("hurt", &hurt);
		g_pScriptSystem->AddMethod("create_entity", &create_entity);
		g_pScriptSystem->AddMethod("spawn_entity", &spawn_entity);
		g_pScriptSystem->AddMethod("get_player_origin", &get_player_origin);
		g_pScriptSystem->AddMethod("get_entity_origin", &get_entity_origin);
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
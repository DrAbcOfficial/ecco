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

#define PLUGIN_VERSION __DATE__ "T" __TIME__

class CEccoPlugin : public IEccoPlugin {
	virtual const char* GetName() const { return "Base"; };
	virtual const char* GetDescription() const { return "Core"; }
	virtual const char* GetVersion() const { return PLUGIN_VERSION; };
	virtual const char* GetAuthor() const { return "Dr.Abc"; };

	void RegisteMethods() {
#pragma region Player Storage Item
		g_pScriptSystem->CreateCommand("Player::GetCredits", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				auto player = g_pEccoFuncs->GetPlayerItem(index);
				auto obj = pThis->NewIntObject(player->GetCredits());
				pThis->SetObjectResult(obj);
			}
			catch(std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
		});
		g_pScriptSystem->CreateCommand("Player::AddCredits", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int money = atoi(argv[1]);
				auto player = g_pEccoFuncs->GetPlayerItem(index);
				player->AddCredits(money);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::SetCredits", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int money = atoi(argv[1]);
				auto player = g_pEccoFuncs->GetPlayerItem(index);
				player->SetCredits(money);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::GetSteamId", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				auto player = g_pEccoFuncs->GetPlayerItem(index);
				auto obj = pThis->NewStringObject(player->GetSteamId(), -1);
				pThis->SetObjectResult(obj);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::GetLang", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				auto player = g_pEccoFuncs->GetPlayerItem(index);
				auto obj = pThis->NewStringObject(player->GetLang(), -1);
				pThis->SetObjectResult(obj);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::GetName", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				auto player = g_pEccoFuncs->GetPlayerItem(index);
				auto obj = pThis->NewStringObject(player->GetName(), -1);
				pThis->SetObjectResult(obj);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::GetAdminLevel", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				auto player = g_pEccoFuncs->GetPlayerItem(index);
				auto obj = pThis->NewIntObject((int)player->GetAdminLevel());
				pThis->SetObjectResult(obj);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::SetAdminLevel", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int admin = atoi(argv[1]);
				auto player = g_pEccoFuncs->GetPlayerItem(index);
				player->SetAdminLevel((ADMIN_LEVEL)admin);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
#pragma endregion
#pragma region Menu Item
		g_pScriptSystem->CreateCommand("Menu::GetId", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				auto menu = g_pEccoFuncs->GetExcutor(index);
				auto obj = pThis->NewStringObject(menu->GetId(), -1);
				pThis->SetObjectResult(obj);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});	
		g_pScriptSystem->CreateCommand("Menu::GetIndex", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
				if (argc < 1)
					return IEccoScriptSystem::Result::Error;
				try {
					int index = atoi(argv[0]);
					auto menu = g_pEccoFuncs->GetExcutor(index);
					auto obj = pThis->NewIntObject(menu->GetIndex());
					pThis->SetObjectResult(obj);
				}
				catch (std::exception ex) {
					pThis->ThrowError(ex.what(), ex.what());
					return IEccoScriptSystem::Result::Error;
				}
				return IEccoScriptSystem::Result::OK;
				});
		g_pScriptSystem->CreateCommand("Menu::GetIndex", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int plr = atoi(argv[1]);
				auto pent = INDEXENT(plr);
				auto menu = g_pEccoFuncs->GetExcutor(index);
				auto obj = pThis->NewStringObject(menu->GetDisplayNameRaw(pent), -1);
				pThis->SetObjectResult(obj);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
#pragma endregion

#pragma region Basic commands
		g_pScriptSystem->CreateCommand("give", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				const char* name = argv[1];
				edict_t* pev = INDEXENT(index);
				edict_t* pent = GiveNamedItem_Common(pev, name);
				if (!pent)
					return IEccoScriptSystem::Result::Return;
				MDLL_Touch(pent, pev);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
		});
		g_pScriptSystem->CreateCommand("saytext", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				const char* contetnt = argv[1];
				edict_t* pev = INDEXENT(index);
				g_pEccoFuncs->ClientPrintf(pev, 3, contetnt);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("saytextall", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				const char* content = argv[0];
				g_pEccoFuncs->ClientPrintfAll(3, content);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("console", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				const char* content = argv[0];
				ALERT(ALERT_TYPE::at_console, content);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("maxhealth", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int value = atoi(argv[1]);
				edict_t* pev = INDEXENT(index);
				pev->v.max_health = static_cast<float>(value);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("maxarmor", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int value = atoi(argv[1]);
				edict_t* pev = INDEXENT(index);
				pev->v.armortype = static_cast<float>(value);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("maxspeed", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int value = atoi(argv[1]);
				edict_t* pev = INDEXENT(index);
				pev->v.maxspeed = static_cast<float>(value);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("gravity", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int value = atoi(argv[1]);
				edict_t* pev = INDEXENT(index);
				pev->v.gravity = static_cast<float>(value);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("heal", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int value = atoi(argv[1]);
				edict_t* pev = INDEXENT(index);
				CBasePlayer* pPlayer = static_cast<CBasePlayer*>(pev->pvPrivateData);
				if (!pPlayer)
					return IEccoScriptSystem::Result::Error;
				pPlayer->TakeHealth(static_cast<float>(value), DMG_MEDKITHEAL, 0);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("armor", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int value = atoi(argv[1]);
				edict_t* pev = INDEXENT(index);
				CBasePlayer* pPlayer = static_cast<CBasePlayer*>(pev->pvPrivateData);
				if (!pPlayer)
					return IEccoScriptSystem::Result::Error;
				pPlayer->TakeArmor(static_cast<float>(value), DMG_MEDKITHEAL, 0);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("hurt", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 2)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				int value = atoi(argv[1]);
				edict_t* pev = INDEXENT(index);
				CBasePlayer* pPlayer = static_cast<CBasePlayer*>(pev->pvPrivateData);
				if (!pPlayer)
					return IEccoScriptSystem::Result::Error;
				pPlayer->TakeDamage(nullptr, nullptr, static_cast<float>(value), DMG_GENERIC);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("create_entity", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				const char* pszName = argv[0];
				int istr = MAKE_STRING(pszName);
				edict_t* pent = CREATE_NAMED_ENTITY(istr);
				if (FNullEnt(pent)) {
					ALERT(at_console, "NULL Ent in CreateNamedEntity!\n");
					return IEccoScriptSystem::Result::Error;
				}
				auto obj = pThis->NewIntObject(ENTINDEX(pent));
				pThis->SetObjectResult(obj);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("spawn_entity", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				if (index < 0)
					return IEccoScriptSystem::Result::Error;
				auto pent = INDEXENT(index);
				if (FNullEnt(pent)) {
					ALERT(at_console, "Try to Spawn a NULL ent!\n");
					return IEccoScriptSystem::Result::Error;
				}
				MDLL_Spawn(pent);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("get_entity_origin", [](IEccoScriptSystem* pThis, int argc, const char* argv[]) {
			if (argc < 1)
				return IEccoScriptSystem::Result::Error;
			try {
				int index = atoi(argv[0]);
				if (index < 0)
					return IEccoScriptSystem::Result::Error;
				auto pent = INDEXENT(index);
				if (FNullEnt(pent)) {
					ALERT(at_console, "Try to get origin a NULL entity!\n");
					return IEccoScriptSystem::Result::Error;
				}
				IEccoScriptSystem::Object* position[3] = {
					pThis->NewDoubleObject(pent->v.origin[0]),
					pThis->NewDoubleObject(pent->v.origin[1]),
					pThis->NewDoubleObject(pent->v.origin[2])
				};
				IEccoScriptSystem::Object* obj = pThis->NewListObject(3, position);
				pThis->SetObjectResult(obj);
			}
			catch (std::exception ex) {
				pThis->ThrowError(ex.what(), ex.what());
				return IEccoScriptSystem::Result::Error;
			}
			return IEccoScriptSystem::Result::OK;
			});
#pragma endregion
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
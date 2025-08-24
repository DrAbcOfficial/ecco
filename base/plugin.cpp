#include "api/IEccoPlugin.h"
#include "sdk_api.h"

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
plugin_info_t* gpPlugin_info;			// master plugin info

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
		g_pScriptSystem->CreateCommand("Player::GetCredits", "int ent_index", "Get player credits", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			auto player = g_pEccoFuncs->GetPlayerItem(index);
			if(!player) {
				interp->ThrowError("NULL player", "can not find player by ent index");
				return IEccoScriptSystem::Result::Error;
			}
			auto obj = interp->NewIntObject(player->GetCredits());
			interp->SetObjectResult(obj);
			return IEccoScriptSystem::Result::OK;
		});
		g_pScriptSystem->CreateCommand("Player::AddCredits", "int ent_index, int value", "Add credits to a player", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			int money = argv[1]->intValue;
			auto player = g_pEccoFuncs->GetPlayerItem(index);
			if (!player) {
				interp->ThrowError("NULL player", "can not find player by ent index");
				return IEccoScriptSystem::Result::Error;
			}
			player->AddCredits(money);
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::SetCredits", "int ent_index, int value", "Set credits for a player", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			int money = argv[1]->intValue;
			auto player = g_pEccoFuncs->GetPlayerItem(index);
			if (!player) {
				interp->ThrowError("NULL player", "can not find player by ent index");
				return IEccoScriptSystem::Result::Error;
			}
			player->SetCredits(money);
			return IEccoScriptSystem::Result::OK;
		});
		g_pScriptSystem->CreateCommand("Player::GetSteamId", "int ent_index", "Get player steamid", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			auto player = g_pEccoFuncs->GetPlayerItem(index);
			if (!player) {
				interp->ThrowError("NULL player", "can not find player by ent index");
				return IEccoScriptSystem::Result::Error;
			}
			auto obj = interp->NewStringObject(player->GetSteamId(), -1);
			interp->SetObjectResult(obj);
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::GetLang", "int ent_index", "Get player's lang code", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			auto player = g_pEccoFuncs->GetPlayerItem(index);
			if (!player) {
				interp->ThrowError("NULL player", "can not find player by ent index");
				return IEccoScriptSystem::Result::Error;
			}
			auto obj = interp->NewStringObject(player->GetLang(), -1);
			interp->SetObjectResult(obj);
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::GetName", "int ent_index", "Get player name", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			auto player = g_pEccoFuncs->GetPlayerItem(index);
			if (!player) {
				interp->ThrowError("NULL player", "can not find player by ent index");
				return IEccoScriptSystem::Result::Error;
			}
			auto obj = interp->NewStringObject(player->GetName(), -1);
			interp->SetObjectResult(obj);
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::GetAdminLevel", "int ent_index", "Get player's admin level", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			auto player = g_pEccoFuncs->GetPlayerItem(index);
			if (!player) {
				interp->ThrowError("NULL player", "can not find player by ent index");
				return IEccoScriptSystem::Result::Error;
			}
			auto obj = interp->NewIntObject((int)player->GetAdminLevel());
			interp->SetObjectResult(obj);
			return IEccoScriptSystem::Result::OK;
			});
		g_pScriptSystem->CreateCommand("Player::SetAdminLevel", "int ent_index, int level", "Set player's admin level", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			int admin = argv[1]->intValue;
			auto player = g_pEccoFuncs->GetPlayerItem(index);
			if (!player) {
				interp->ThrowError("NULL player", "can not find player by ent index");
				return IEccoScriptSystem::Result::Error;
			}
			player->SetAdminLevel((ADMIN_LEVEL)admin);
			return IEccoScriptSystem::Result::OK;
			});
#pragma endregion
#pragma region Menu Item
	g_pScriptSystem->CreateCommand("Menu::GetId", "int menu_index", "Get menu's Id", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		auto menu = g_pEccoFuncs->GetMenuExcutor(index);
		if (!menu) {
			interp->ThrowError("NULL menu", "can not find menu by menu index");
			return IEccoScriptSystem::Result::Error;
		}
		auto obj = interp->NewStringObject(menu->GetId(), -1);
		interp->SetObjectResult(obj);
		return IEccoScriptSystem::Result::OK;
	});	
	g_pScriptSystem->CreateCommand("Menu::GetIndex", "int menu_index", "Get menu's index", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		auto menu = g_pEccoFuncs->GetMenuExcutor(index);
		if (!menu) {
			interp->ThrowError("NULL menu", "can not find menu by menu index");
			return IEccoScriptSystem::Result::Error;
		}
		auto obj = interp->NewIntObject(menu->GetIndex());
		interp->SetObjectResult(obj);
		return IEccoScriptSystem::Result::OK;
	});
	g_pScriptSystem->CreateCommand("Menu::GetDisplay", "int menu_index, int ent_index", "Get menu's display name to a player", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		int plr = argv[1]->intValue;
		auto pent = INDEXENT(plr);
		auto menu = g_pEccoFuncs->GetMenuExcutor(index);
		if (!menu) {
			interp->ThrowError("NULL menu", "can not find menu by menu index");
			return IEccoScriptSystem::Result::Error;
		}
		auto obj = interp->NewStringObject(menu->GetDisplayNameRaw(pent), -1);
		interp->SetObjectResult(obj);
		return IEccoScriptSystem::Result::OK;
	});
#pragma endregion

#pragma region Basic commands
	g_pScriptSystem->CreateCommand("give", "int ent_index, string item_name", "Give named item to a player", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			const char* name = argv[1]->strValue;
			edict_t* pev = INDEXENT(index);
			if (FNullEnt(pev)) {
				interp->ThrowError("NULL ent", "can not find ent with gived index");
				return IEccoScriptSystem::Result::Error;
			}
			edict_t* pent = GiveNamedItem_Common(pev, name);
			if (FNullEnt(pent)) {
				interp->ThrowError("NULL give item", "NULL with give named item");
				return IEccoScriptSystem::Result::Error;
			}	
			MDLL_Touch(pent, pev);
			return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("saytext", "int ent_index, string content", "Say something to a player", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			int index = argv[0]->intValue;
			const char* contetnt = argv[1]->strValue;
			edict_t* pev = INDEXENT(index);
			if (FNullEnt(pev)) {
				interp->ThrowError("NULL ent", "can not find ent with gived index");
				return IEccoScriptSystem::Result::Error;
			}
			g_pEccoFuncs->ClientPrintf(pev, 3, contetnt);
			return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("saytextall", "string content", "Say something to all players", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			const char* content = argv[0]->strValue;
			g_pEccoFuncs->ClientPrintfAll(3, content);
			return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("console", "string content", "print something to console", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
			const char* content = argv[0]->strValue;
			ALERT(ALERT_TYPE::at_console, content);
			return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("maxhealth", "int ent_index, int value", "Set max health for a entity", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		int value = argv[1]->intValue;
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "can not find ent with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		pent->v.max_health = static_cast<float>(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("maxarmor", "int ent_index, int value", "Set max armor for a entity", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		int value = argv[1]->intValue;
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "can not find ent with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		pent->v.armortype = static_cast<float>(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("maxspeed", "int ent_index, int value", "Set max speed for a entity", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		int value = argv[1]->intValue;
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "can not find ent with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		pent->v.maxspeed = static_cast<float>(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("gravity", "int ent_index, float value", "Set gravity for a entity (%)", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		float value = argv[1]->floatValue;
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "can not find ent with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		pent->v.gravity = value;
		return IEccoScriptSystem::Result::OK;
	});
	g_pScriptSystem->CreateCommand("heal", "int ent_index, int value", "heal a entity", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		int value = argv[1]->intValue;
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "can not find ent with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		CBaseEntity* pEntity = static_cast<CBasePlayer*>(pent->pvPrivateData);
		if (!pEntity) {
			interp->ThrowError("NULL entity", "entity privateData is NULL with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		pEntity->TakeHealth(static_cast<float>(value), DMG_MEDKITHEAL, 0);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("armor", "int ent_index, int value", "armor a entity", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		int value = argv[1]->intValue;
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "can not find ent with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		CBaseEntity* pEntity = static_cast<CBasePlayer*>(pent->pvPrivateData);
		if (!pEntity) {
			interp->ThrowError("NULL entity", "entity privateData is NULL with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		pEntity->TakeArmor(static_cast<float>(value), DMG_MEDKITHEAL, 0);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("hurt", "int ent_index, int value", "hurt a entity", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		int value = argv[1]->intValue;
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "can not find ent with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		CBaseEntity* pEntity = static_cast<CBasePlayer*>(pent->pvPrivateData);
		if (!pEntity) {
			interp->ThrowError("NULL entity", "entity privateData is NULL with gived index");
			return IEccoScriptSystem::Result::Error;
		}
		pEntity->TakeDamage(nullptr, nullptr, static_cast<float>(value), DMG_GENERIC);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("create_entity", "string classname", "create a entity", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		const char* pszName = argv[0]->strValue;
		int istr = MAKE_STRING(pszName);
		edict_t* pent = CREATE_NAMED_ENTITY(istr);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "NULL Ent in CreateNamedEntity!");
			return IEccoScriptSystem::Result::Error;
		}
		auto obj = interp->NewIntObject(ENTINDEX(pent));
		interp->SetObjectResult(obj);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("spawn_entity", "int ent_index", "spawn a entity", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		if (index < 0)
			return IEccoScriptSystem::Result::Error;
		auto pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "Try to Spawn a NULL ent!");
			return IEccoScriptSystem::Result::Error;
		}
		MDLL_Spawn(pent);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("get_entity_origin", "int ent_index", "get a entity origin", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		if (index < 0)
			return IEccoScriptSystem::Result::Error;
		auto pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "Try to get origin a NULL entity!");
			return IEccoScriptSystem::Result::Error;
		}
		IEccoScriptSystem::Object* position[3] = {
			interp->NewDoubleObject(pent->v.origin[0]),
			interp->NewDoubleObject(pent->v.origin[1]),
			interp->NewDoubleObject(pent->v.origin[2])
		};
		IEccoScriptSystem::Object* obj = interp->NewListObject(3, position);
		interp->SetObjectResult(obj);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("respawn_player", "int ent_index, float tillRevive", "respawn a player", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int index = argv[0]->intValue;
		float revive = argv[1]->floatValue;
		if (index < 0)
			return IEccoScriptSystem::Result::Error;
		auto pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			interp->ThrowError("NULL ent", "Try to respawn a NULL player!");
			return IEccoScriptSystem::Result::Error;
		}
		CBasePlayer* pPlayer = static_cast<CBasePlayer*>(pent->pvPrivateData);
		if (!pPlayer->IsPlayer()) {
			interp->ThrowError("NULL ent", "Try to respawn is NOT a player!");
			return IEccoScriptSystem::Result::Error;
		}
		pPlayer->BeginRevive(revive);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("con_command", "string cmd", "send a cmd to console", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		const char* cmd = argv[0]->strValue;
		if (!cmd || strlen(cmd) < 0) {
			interp->ThrowError("NULL string", "Try to submit a NULL string!");
			return IEccoScriptSystem::Result::Error;
		}
		SERVER_COMMAND(const_cast<char*>(cmd));
		return IEccoScriptSystem::Result::OK;
		});

#pragma endregion

#pragma region NewworkMessage
	g_pScriptSystem->CreateCommand("NetMsg::Begin", "int msg_dest, int msg_type, float msg_org_x, float msg_org_y, float msg_org_z, int target", "Begin a network message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int msg_dest = argv[0]->intValue;
		int msg_type = argv[1]->intValue;
		float msg_org[3] = {
			argv[2]->floatValue,
			argv[3]->floatValue,
			argv[4]->floatValue
		};
		int target = argv[5]->intValue;
		auto pent = INDEXENT(target);
		MESSAGE_BEGIN(msg_dest, msg_type, msg_org, pent);
		return IEccoScriptSystem::Result::OK;
	});
	g_pScriptSystem->CreateCommand("NetMsg::WriteByte", "int value", "Write a byte to message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int value = argv[0]->intValue;
		WRITE_BYTE(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("NetMsg::WriteChar", "int value", "Write a char to message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int value = argv[0]->intValue;
		WRITE_CHAR(value);
		return IEccoScriptSystem::Result::OK;
	});
	g_pScriptSystem->CreateCommand("NetMsg::WriteShort", "int value", "Write a short to message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int value = argv[0]->intValue;
		WRITE_SHORT(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("NetMsg::WriteLong", "int value", "Write a long to message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int value = argv[0]->intValue;
		WRITE_LONG(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("NetMsg::WriteAngle", "float value", "Write a angle to message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		float value = argv[0]->floatValue;
		WRITE_ANGLE(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("NetMsg::WriteCoord", "float value", "Write a coord to message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		float value = argv[0]->floatValue;
		WRITE_COORD(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("NetMsg::WriteString", "string value", "Write a string to message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		const char* value = argv[0]->strValue;
		WRITE_STRING(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("NetMsg::WriteEntity", "int value", "Write a entity to message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		int value = argv[0]->intValue;
		WRITE_ENTITY(value);
		return IEccoScriptSystem::Result::OK;
		});
	g_pScriptSystem->CreateCommand("NetMsg::End", "", "End a network message", [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv) {
		MESSAGE_END();
		return IEccoScriptSystem::Result::OK;
	});
#pragma endregion

	}

	virtual void Query(IEccoScriptSystem* script_system, IEccoFuncs* ecco_funcs,
		enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals,
		meta_globals_t* penMetaGlobals, gamedll_funcs_t* penGamedllFuncs, mutil_funcs_t* penMetaUtilFuncs,
		plugin_info_t* pPluginInfo) {
		g_pEccoFuncs = ecco_funcs;
		g_pScriptSystem = script_system;
		memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
		gpGlobals = pGlobals;
		gpMetaGlobals = penMetaGlobals;
		gpGamedllFuncs = penGamedllFuncs;
		gpMetaUtilFuncs = penMetaUtilFuncs;
		gpPlugin_info = pPluginInfo;
	};

	virtual void Initialize() {
		RegisteMethods();
	};

	virtual void Shutdown() {};
};
static CEccoPlugin s_EccoPluginInstance;

ECCO_API IEccoPlugin* GetEccoPluginInstance() {
	return &s_EccoPluginInstance;
}
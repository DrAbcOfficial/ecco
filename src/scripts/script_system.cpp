#include <filesystem>
#include <array>
#include <ChaiScript/chaiscript.hpp>

#include <extdll.h>
#include <meta_api.h>

#include "meta_utility.h"

#include "obj/CChaiPlayer.h"

#include "basescriptmethods.h"

#include "scripts/script_system.h"

std::vector<CEccoScriptItem*> g_aryEccoScriptItems;

static std::array<CChaiPlayer, 32> s_aryCallerWrappers;
static chaiscript::ChaiScript g_ScriptEngine;
void InitScriptEngine(){
	chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
	chaiscript::utility::add_class<CChaiPlayer>(*m,
		"CChaiPlayer", 
		{
		}, 
		{
			{chaiscript::fun(&CChaiPlayer::m_szName), "m_szName"},
			{chaiscript::fun(&CChaiPlayer::m_szSteamId), "m_szSteamId"}
		}
	);
	g_ScriptEngine.add(m);

	g_ScriptEngine.add(chaiscript::fun(&EccoBaseScriptMethods::give), "give");
}

bool EvalScriptContent(edict_t* caller, std::string& content){
	int index = ENTINDEX(caller) - 1;
	CChaiPlayer* player = &s_aryCallerWrappers[index];
	player->m_pEdict = caller;
	player->m_szName = STRING(VARS(caller)->netname);
	player->m_szSteamId = GetPlayerSteamId(caller);
	std::map<std::string, chaiscript::Boxed_Value> locals;
	locals["caller"] = chaiscript::var(player);
	g_ScriptEngine.set_locals(locals);
	g_ScriptEngine.eval(content);
	return true;
}

void ResetEccoScriptItems(){
	for (CEccoScriptItem* item : g_aryEccoScriptItems) {
		delete item;
	}
	g_aryEccoScriptItems.clear();
}

bool LoadEccoScriptItems(){
	std::filesystem::path gamedir(GetGameDir());
	gamedir.append("addons/ecco/scripts");
	if (!std::filesystem::exists(gamedir)) {
		LOG_CONSOLE(PLID, "Ecco script directory does not exist: %s", gamedir.string().c_str());
		return false;
	}
	for (const auto& entry : std::filesystem::directory_iterator(gamedir)) {
		if (entry.is_regular_file() && entry.path().extension() == ".toml") 
			g_aryEccoScriptItems.push_back(new CEccoScriptItem(entry.path().string()));
	}
	return true;
}

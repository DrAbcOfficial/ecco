#include <filesystem>
#include <array>
#include <extdll.h>

#include "storage/Storage.h"
#include "script_system.h"
#include "CEccoScriptSystem.h"

#include <meta_api.h>
#include "meta_utility.h"

std::vector<CEccoScriptItem*> g_aryEccoScriptItems;

CEccoScriptSystem* g_ScriptSystem = nullptr;

void InitializeScriptSystem(){
	g_ScriptSystem = new CEccoScriptSystem();
}

IEccoScriptSystem::Result EvalScriptContent(edict_t* caller, CEccoScriptExecutor* pexcuter){
	g_ScriptSystem->ResetEnviroment();
	auto index = g_ScriptSystem->NewIntObject(ENTINDEX(caller));
	g_ScriptSystem->SetValue("caller_index", index);
	index = g_ScriptSystem->NewIntObject(pexcuter->m_iIndex);
	g_ScriptSystem->SetValue("item_index", index);
	auto ret = g_ScriptSystem->Eval(pexcuter->m_szScript.c_str());
	g_ScriptSystem->UnsetValue("caller_index");
	g_ScriptSystem->UnsetValue("item_index");
	return ret;
}

void ResetEccoScriptItems(){
	for (CEccoScriptItem* item : g_aryEccoScriptItems) {
		delete item;
	}
	g_aryEccoScriptItems.clear();
}

bool LoadEccoScriptItems(){
	std::filesystem::path gamedir(EccoMetaUtility::GetGameDir());
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

void PrecacheAllScriptItems(){
	for (auto& item : g_aryEccoScriptItems) {
		for (auto& str : item->m_aryPrecaches) {
			PRECACHE_MODEL(const_cast<char*>(str.c_str()));
		}
		for (auto& str : item->m_arySoundPrecaches) {
			PRECACHE_SOUND(const_cast<char*>(str.c_str()));
		}
	}
}

IEccoScriptSystem* GetEccoScriptSystem() {
	return g_ScriptSystem;
}
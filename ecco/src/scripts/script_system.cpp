#include <filesystem>
#include <array>
#include <algorithm>
#include <extdll.h>

#include "storage/Storage.h"
#include "script_system.h"
#include "CEccoScriptSystem.h"

#include <meta_api.h>
#include "meta_utility.h"

#undef min
#undef max

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

	static auto getCharWeight = [](char c) -> int {
		if (isdigit(c))
			return 1 + (c - '0');
		else if (islower(c))
			return 11 + (c - 'a');
		else if (isupper(c))
			return 37 + (c - 'A');
		return 100;
	};
	static auto compareIds = [](const std::string& a, const std::string& b) {
		size_t minLen = std::min(a.length(), b.length());
		for (size_t i = 0; i < minLen; ++i) {
			int weightA = getCharWeight(a[i]);
			int weightB = getCharWeight(b[i]);
			if (weightA != weightB)
				return weightA < weightB;
		}
		return a.length() < b.length();
		};
	std::sort(g_aryEccoScriptItems.begin(), g_aryEccoScriptItems.end(),
		[](CEccoScriptItem* a1, CEccoScriptItem* a2) {
			return compareIds(a1->m_szId, a2->m_szId);
		}
	);
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
		for (auto& str : item->m_aryOtherPrecaches) {
			PRECACHE_GENERIC(const_cast<char*>(str.c_str()));
		}
	}
}

IEccoScriptSystem* GetEccoScriptSystem() {
	return g_ScriptSystem;
}
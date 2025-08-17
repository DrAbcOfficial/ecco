#include <filesystem>
#include <array>
#include <extdll.h>

#include "storage/Storage.h"
#include "script_system.h"
#include "CEccoScriptSystem.h"

#include <meta_api.h>
#include "meta_utility.h"

std::vector<CEccoScriptItem*> g_aryEccoScriptItems;

CEccoScriptSystem g_ScriptSystem;

void InitScriptEngine(){
	auto engine = g_ScriptSystem.GetScriptEngine();
	engine->add(chaiscript::type_conversion<const char*, std::string>());
	chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
	chaiscript::utility::add_class<IPlayerStorageItem>(*m,
		"CEccoPlayer", 
		{
		}, 
		{
			{chaiscript::fun(&IPlayerStorageItem::GetName), "GetName"},
			{chaiscript::fun(&IPlayerStorageItem::GetSteamId), "GetSteamId"},

			{chaiscript::fun(&IPlayerStorageItem::GetCredits), "GetCredits"},
			{chaiscript::fun(&IPlayerStorageItem::SetCredits), "SetCredits"},
			{chaiscript::fun(&IPlayerStorageItem::AddCredits), "AddCredits"},

			{chaiscript::fun(&IPlayerStorageItem::GetAdminLevel), "GetAdminLevel"},
			{chaiscript::fun(&IPlayerStorageItem::SetAdminLevel), "SetAdminLevel"},

			{chaiscript::fun(&IPlayerStorageItem::GetLang), "GetLang"},
			{chaiscript::fun(&IPlayerStorageItem::SetLang), "SetLang"}

			
		}
	);
	chaiscript::utility::add_class<CEccoScriptExecutor>(*m,
		"CEccoBuyItem",
		{
		},
		{
			{chaiscript::fun(&CEccoScriptExecutor::m_iCost), "m_iCost"},
			{chaiscript::fun(&CEccoScriptExecutor::m_bitFlags), "m_bitFlags"},
			{chaiscript::fun(&CEccoScriptExecutor::m_szId), "m_szId"},
			{chaiscript::fun(&CEccoScriptExecutor::GetDisplayNameForChai), "GetDisplayName"},
		}
	);
	engine->add(m);
}

EvalResult EvalScriptContent(edict_t* caller, CEccoScriptExecutor* pexcuter){
	auto engine = g_ScriptSystem.GetScriptEngine();
	std::map<std::string, chaiscript::Boxed_Value> locals;
	locals["caller"] = chaiscript::var((IPlayerStorageItem*)GetPlayerStorageItem(caller));
	locals["buy_item"] = chaiscript::var(pexcuter);
	locals["ret"] = chaiscript::var(false); // Default return value
	engine->set_locals(locals);
	try {
		engine->eval(pexcuter->m_szScript);
	}
	catch (const chaiscript::exception::eval_error& e) {
		std::string error_msg = "ChaiScript ";
		error_msg += pexcuter->m_szId;
		error_msg += " eval error:\n";
		error_msg += e.pretty_print().c_str();
		LOG_ERROR(PLID, error_msg.c_str());
		return EvalResult::Error;
	}
	catch (const std::exception& e) {
		std::string error_msg = "ChaiScript ";
		error_msg += pexcuter->m_szId;
		error_msg += " exception: \t";
		error_msg += e.what();
		LOG_ERROR(PLID, error_msg.c_str());
		return EvalResult::Error;
	}
	catch (...) {
		std::string error_msg = "ChaiScript ";
		error_msg += pexcuter->m_szId;
		error_msg += " unknown exception occurred.";
		LOG_ERROR(PLID, error_msg.c_str());
		return EvalResult::Error;
	}
	bool ret = chaiscript::boxed_cast<bool>(engine->get_locals()["ret"]);
	return ret ? EvalResult::Success : EvalResult::Failure;
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
	return &g_ScriptSystem;
}
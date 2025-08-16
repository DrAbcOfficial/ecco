#include <filesystem>
#include <array>
#include <ChaiScript/chaiscript.hpp>

#include <extdll.h>
#include <meta_api.h>

#include "meta_utility.h"

#include "storage/Storage.h"

#include "basescriptmethods.h"

#include "scripts/script_system.h"

std::vector<CEccoScriptItem*> g_aryEccoScriptItems;

static chaiscript::ChaiScript g_ScriptEngine;
void InitScriptEngine(){
	chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
	chaiscript::utility::add_class<CPlayerStorageItem>(*m,
		"CEccoPlayer", 
		{
		}, 
		{
			{chaiscript::fun(&CPlayerStorageItem::GetSteamId), "GetSteamId"},
			{chaiscript::fun(&CPlayerStorageItem::GetCredits), "GetCredits"},
			{chaiscript::fun(&CPlayerStorageItem::GetName), "GetName"},
			{chaiscript::fun(&CPlayerStorageItem::GetLang), "GetLang"},
			{chaiscript::fun(&CPlayerStorageItem::SetCredits), "SetCredits"},
			{chaiscript::fun(&CPlayerStorageItem::AddCredits), "AddCredits"},
			{chaiscript::fun(&CPlayerStorageItem::m_iScore), "m_iScore"}
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
	g_ScriptEngine.add(m);

#define ADD_METHOD(method) g_ScriptEngine.add(chaiscript::fun(&EccoBaseScriptMethods::method), #method)
	ADD_METHOD(give);
	ADD_METHOD(saytext);
	ADD_METHOD(saytextall);
	ADD_METHOD(addmoney);
	ADD_METHOD(setmoney);
	ADD_METHOD(getmoney);
	ADD_METHOD(maxhealth);
	ADD_METHOD(maxarmor);
	ADD_METHOD(maxspeed);
	ADD_METHOD(gravity);
}

EvalResult EvalScriptContent(edict_t* caller, CEccoScriptExecutor* pexcuter){
	std::map<std::string, chaiscript::Boxed_Value> locals;
	locals["caller"] = chaiscript::var(GetPlayerStorageItem(caller));
	locals["buy_item"] = chaiscript::var(pexcuter);
	locals["ret"] = chaiscript::var(false); // Default return value
	g_ScriptEngine.set_locals(locals);
	try {
		g_ScriptEngine.eval(pexcuter->m_szScript);
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
	bool ret = chaiscript::boxed_cast<bool>(g_ScriptEngine.get_locals()["ret"]);
	return ret ? EvalResult::Success : EvalResult::Failure;
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
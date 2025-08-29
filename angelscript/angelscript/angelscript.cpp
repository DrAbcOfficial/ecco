#include <vector>

#include <extdll.h>
#include <meta_api.h>
#include "angelscript.hpp"
#include "api/IEccoPlugin.h"

extern IEccoScriptSystem* g_pScriptSystem;
extern IEccoFuncs* g_pEccoFuncs;

angelhook_t g_AngelHook;

class IASPlayerStorageItem : public IPlayerStorageItem {
	public:
	void GetLangAS(CString* lang) {
		const char* l = GetLang();
		lang->assign(l, strlen(l));
	}
	void SetLangAS(CString* lang) {
		SetLang(lang->c_str());
	}
};

class IASEccoBaseExcutor : public IEccoBaseExcutor {
public:
	void ASExcute(void* pPlayer, int selection) {
		edict_t* plr = g_pEccoFuncs->PrivateToEdict(pPlayer);
		if (FNullEnt(plr))
			return;
		Excute(plr, selection);
	}
	void ASGetDisplayNameRaw(CString* str, void* pPlyaer) {
		edict_t* plr = g_pEccoFuncs->PrivateToEdict(pPlyaer);
		if (FNullEnt(plr))
			return;
		const char* name = GetDisplayNameRaw(plr);
		str->assign(name, strlen(name));
	}
	void ASGetId(CString* str) {
		const char* id = GetId();
		str->assign(id, strlen(id));
	}
};

class CTCLArgs {
private:
	CString* asStr = nullptr;
	void ReleaseString() {
		if (asStr) {
			static auto serverManager = ASEXT_GetServerManager();
			static auto as_engine = serverManager->scriptEngine;
			static auto str_type = as_engine->GetTypeInfoByDecl("string");
			as_engine->ReleaseScriptObject(asStr, str_type);
			asStr = nullptr;
		}
	}
	void CreateString() {
		ReleaseString();
		static auto serverManager = ASEXT_GetServerManager();
		static auto as_engine = serverManager->scriptEngine;
		static auto str_type = as_engine->GetTypeInfoByDecl("string");
		asStr = static_cast<CString*>(as_engine->CreateScriptObject(str_type));
	}

public:
	~CTCLArgs() {
		ReleaseString();
	}
	int type;
	union {
		int intValue;
		const char* strValue;
		double doubleValue;
		float floatValue;
		bool boolValue;
	};
	int GetType() const { return type; }
	int GetInt() const { return type == static_cast<int>(IEccoScriptSystem::ScriptArgType::Int) ? intValue : 0; }
	CString* GetString(){
		if (type != static_cast<int>(IEccoScriptSystem::ScriptArgType::String)) {
			return nullptr;
		}
		CreateString();
		asStr->assign(strValue, strlen(strValue));
		return asStr;
	}
	double GetDouble() const { return type == static_cast<int>(IEccoScriptSystem::ScriptArgType::Double) ? doubleValue : 0.0; }
	float GetFloat() const { return type == static_cast<int>(IEccoScriptSystem::ScriptArgType::Float) ? floatValue : 0.0f; }
	bool GetBoolean() const { return type == static_cast<int>(IEccoScriptSystem::ScriptArgType::Boolean) ? boolValue : false; }
};

class CEccoFunc{
public:
	IASPlayerStorageItem* GetPlayerStorage(int index){
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent))
			return nullptr;
		return reinterpret_cast<IASPlayerStorageItem*>(g_pEccoFuncs->GetPlayerItem(index));
	}
	IASPlayerStorageItem* GetPlayerStoragePrivate(void* player) {
		edict_t* plr = g_pEccoFuncs->PrivateToEdict(player);
		if (FNullEnt(plr))
			return nullptr;
		int index = ENTINDEX(plr);
		return reinterpret_cast<IASPlayerStorageItem*>(g_pEccoFuncs->GetPlayerItem(index));
	}
	IASEccoBaseExcutor* GetMenuByIndex(int index) {
		auto menu = g_pEccoFuncs->GetMenuExcutor(index);
		return static_cast<IASEccoBaseExcutor*>(menu);
	}

	void RegisterEccoScriptFunction(CString* name, CString* args, CString* description, void* asfunc) {
		g_pScriptSystem->CreateCommand(name->c_str(), args->c_str(), description->c_str(), [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv, void* user_args) {
			int index = argv[0]->intValue;
			int menu_index = argv[1]->intValue;
			IPlayerStorageItem* item = g_pEccoFuncs->GetPlayerItem(index);
			IEccoBaseExcutor* menu = g_pEccoFuncs->GetMenuExcutor(menu_index);
			if (!item || !menu)
				return IEccoScriptSystem::Result::Error;
			
			//读取剩下的参数，并将其打包为参数列表传递
			static auto serverManager = ASEXT_GetServerManager();
			static auto as_engine = serverManager->scriptEngine;
			static asIScriptContext* ctx = as_engine->RequestContext();
			static auto arr_type = as_engine->GetTypeInfoByDecl("array<CTCLArgs@>");
			static auto arr = as_engine->CreateScriptObject(arr_type);
			static asIScriptFunction* funcAryInsert = arr_type->GetMethodByName("insertLast");

			std::vector<CTCLArgs*> arg_list{};
			int remain_argc = argc - 2;
			for(int i = 0; i < remain_argc; i++) {
				CTCLArgs* arg = new CTCLArgs();
				switch (argv[i + 2]->type) {
				case IEccoScriptSystem::ScriptArgType::Int:
					arg->type = static_cast<int>(IEccoScriptSystem::ScriptArgType::Int);
					arg->intValue = argv[i + 2]->intValue;
					break;
				case IEccoScriptSystem::ScriptArgType::String:
					arg->type = static_cast<int>(IEccoScriptSystem::ScriptArgType::String);
					arg->strValue = argv[i + 2]->strValue;
					break;
				case IEccoScriptSystem::ScriptArgType::Double:
					arg->type = static_cast<int>(IEccoScriptSystem::ScriptArgType::Double);
					arg->doubleValue = argv[i + 2]->doubleValue;
					break;
				case IEccoScriptSystem::ScriptArgType::Float:
					arg->type = static_cast<int>(IEccoScriptSystem::ScriptArgType::Float);
					arg->floatValue = argv[i + 2]->floatValue;
					break;
				case IEccoScriptSystem::ScriptArgType::Boolean:
					arg->type = static_cast<int>(IEccoScriptSystem::ScriptArgType::Boolean);
					arg->boolValue = argv[i + 2]->boolValue;
					break;
				default:
					arg->type = static_cast<int>(IEccoScriptSystem::ScriptArgType::Unknown);
					break;
				}
				ctx->Prepare(funcAryInsert);
				ctx->SetObject(arr);
				ctx->SetArgObject(0, &arg);
				ctx->Execute();
				arg_list.push_back(arg);
			}
			
			ctx->Prepare(reinterpret_cast<asIScriptFunction*>(user_args));
			ctx->SetArgObject(0, &item);
			ctx->SetArgObject(1, &menu);
			ctx->SetArgObject(2, &arr);
			ctx->Execute();

			as_engine->ReleaseScriptObject(arr, arr_type);
			for(auto arg : arg_list) {
				delete arg;
			}
			return IEccoScriptSystem::Result::OK;
			}, asfunc);
	}
	void UnregisterEccoScriptFunction(CString* name) {
		g_pScriptSystem->RemoveCommand(name->c_str());
	}
};
CEccoFunc g_pASEccoFunc;

void RegisterAngelScriptMethods(){
	ASEXT_RegisterDocInitCallback([](CASDocumentation* pASDoc) {
		asSFuncPtr reg;
		ASEXT_RegisterObjectType(pASDoc, "Ecco player storage", "CEccoPlayerStorage", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player credits", "CEccoPlayerStorage", "int GetCredits()", IASPlayerStorageItem, GetCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player credits", "CEccoPlayerStorage", "void SetCredits(int credits)", IASPlayerStorageItem, SetCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Add player credits", "CEccoPlayerStorage", "void AddCredits(int credits)", IASPlayerStorageItem, AddCredits, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player language", "CEccoPlayerStorage", "void GetLang(string&out lang)", IASPlayerStorageItem, GetLangAS, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player language", "CEccoPlayerStorage", "void SetLang(string&in lang)", IASPlayerStorageItem, SetLangAS, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player ecco admin level", "CEccoPlayerStorage", "int GetAdminLevel()", IASPlayerStorageItem, GetAdminLevel, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Set player ecco admin level", "CEccoPlayerStorage", "void SetAdminLevel(int level)", IASPlayerStorageItem, SetAdminLevel, asCALL_THISCALL);


		ASEXT_RegisterObjectType(pASDoc, "Ecco menu excutor", "CEccoMenuExcutor", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Excute a ecco menu", "CEccoMenuExcutor", "void Excute(CBasePlayer@ pPlayer, int selection)", IASEccoBaseExcutor, ASExcute, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get display name to a player", "CEccoMenuExcutor", "void GetDisplayName(string& out display, CBasePlayer@ pPlayer)", IASEccoBaseExcutor, ASGetDisplayNameRaw, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Id", "CEccoMenuExcutor", "void GetId(string& out id)", IASEccoBaseExcutor, ASGetId, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Index", "CEccoMenuExcutor", "uint GetIndex()", IASEccoBaseExcutor, GetIndex, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Cost", "CEccoMenuExcutor", "int GetCost()", IASEccoBaseExcutor, GetCost, asCALL_THISCALL);


		ASEXT_RegisterEnum(pASDoc, "TCL args Type", "TCLArgsType", 0);
		ASEXT_RegisterEnumValue(pASDoc, "Nothing or error type", "TCLArgsType", "Unknown", static_cast<int>(IEccoScriptSystem::ScriptArgType::Unknown));
		ASEXT_RegisterEnumValue(pASDoc, "Integer type", "TCLArgsType", "Int", static_cast<int>(IEccoScriptSystem::ScriptArgType::Int));
		ASEXT_RegisterEnumValue(pASDoc, "String type", "TCLArgsType", "String", static_cast<int>(IEccoScriptSystem::ScriptArgType::String));
		ASEXT_RegisterEnumValue(pASDoc, "Double type", "TCLArgsType", "Double", static_cast<int>(IEccoScriptSystem::ScriptArgType::Double));
		ASEXT_RegisterEnumValue(pASDoc, "Float type", "TCLArgsType", "Float", static_cast<int>(IEccoScriptSystem::ScriptArgType::Float));
		ASEXT_RegisterEnumValue(pASDoc, "Boolean type", "TCLArgsType", "Boolean", static_cast<int>(IEccoScriptSystem::ScriptArgType::Boolean));


		// 注册AS回调到TCL
		ASEXT_RegisterFuncDef(pASDoc, "Ecco menu func callback", "bool fnEccoFuncCallback(CEccoPlayerStorage@ pPlayer, CEccoMenuExcutor@ pItem, array<CTCLArgs@> pParams)");
		ASEXT_RegisterObjectType(pASDoc, "Ecco call arg class", "CTCLArgs", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get argument type", "CTCLArgs", "int GetType() const", CTCLArgs, GetType, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get integer argument", "CTCLArgs", "int GetInt() const", CTCLArgs, GetInt, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get string argument", "CTCLArgs", "string GetString()", CTCLArgs, GetString, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get double argument", "CTCLArgs", "double GetDouble() const", CTCLArgs, GetDouble, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get float argument", "CTCLArgs", "float GetFloat() const", CTCLArgs, GetFloat, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get boolean argument", "CTCLArgs", "bool GetBoolean() const", CTCLArgs, GetBoolean, asCALL_THISCALL);


		ASEXT_RegisterObjectType(pASDoc, "Ecco functions", "CEccoFuncs", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco storage", "CEccoFuncs", "CEccoPlayerStorage@ GetEccoStorage(CBasePlayer@ pPlayer)", CEccoFunc, GetPlayerStoragePrivate, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco storage", "CEccoFuncs", "CEccoPlayerStorage@ GetEccoStorage(int player_index)", CEccoFunc, GetPlayerStorage, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco menu", "CEccoFuncs", "CEccoMenuExcutor@ GetMenuByIndex(int menu_index)", CEccoFunc, GetMenuByIndex, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Register Ecco script function", "CEccoFuncs", "void RegisterEccoScriptFunction(string&in name, string&in args, string&in description, fnEccoFuncCallback@ func)", CEccoFunc, RegisterEccoScriptFunction, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Unregister Ecco script function", "CEccoFuncs", "void UnregisterEccoScriptFunction(string&in name)", CEccoFunc, UnregisterEccoScriptFunction, asCALL_THISCALL);

		ASEXT_RegisterGlobalProperty(pASDoc, "Ecco functions", "CEccoFuncs g_EccoFuncs", &g_pASEccoFunc);
	});
}

void RegisterAngelScriptHooks(){
	AS_CREATE_HOOK(PlayerCreditsChanged, "call before player credits changed", 
		"Player", "PlayerCreditsChanged", "CBasePlayer@ pPlayer, int oldCredits, int newCredits, int& out outCredits");
	AS_CREATE_HOOK(MenuExcuted, "call after menu excuted",
		"Menu", "MenuExcuted", "CEccoMenuExcutor@ pMenu, CBasePlayer@ pPlayer");
	AS_CREATE_HOOK(PlayerScoreToCredits, "call before player score to credits",
		"Player", "PlayerScoreToCredits", "CBasePlayer@ pPlayer, int score, int added, int& out newadded");
}
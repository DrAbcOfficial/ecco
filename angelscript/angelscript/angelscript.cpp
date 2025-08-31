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

	void* GetPlayerAS() {
		return GetPlayer()->pvPrivateData;
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

#pragma region TCL
class CTCLArgItem {
private:
public:
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
	void GetString(CString* str) {
		if (type == static_cast<int>(IEccoScriptSystem::ScriptArgType::String))
			str->assign(strValue, strlen(strValue));
	}
	double GetDouble() const { return type == static_cast<int>(IEccoScriptSystem::ScriptArgType::Double) ? doubleValue : 0.0; }
	float GetFloat() const { return type == static_cast<int>(IEccoScriptSystem::ScriptArgType::Float) ? floatValue : 0.0f; }
	bool GetBoolean() const { return type == static_cast<int>(IEccoScriptSystem::ScriptArgType::Boolean) ? boolValue : false; }
};
class CTCLArgList {
public:
	~CTCLArgList() {
		for (auto arg : m_aryArgs) {
			delete arg;
		}
	}
	size_t GetSize() {
		return m_aryArgs.size();
	}
	CTCLArgItem* GetItem(size_t index) {
		if (index >= GetSize())
			return nullptr;
		return m_aryArgs[index];
	}
	void Push(CTCLArgItem* item) {
		m_aryArgs.push_back(item);
	}
private:
	std::vector<CTCLArgItem*> m_aryArgs;
};

#pragma endregion


class CEccoFunc{
private:
	struct as_tcl_funcs_s {
		char name[64];
		char args[256];
		char description[256];
		void* asfunc;
	};
	std::vector<as_tcl_funcs_s*> as_tcl_funcs;
public:
	void CleanTCLFuncs() {
		for (auto f : as_tcl_funcs) {
			delete f;
		}
		as_tcl_funcs.clear();
	}
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

	void RegisterEccoScriptFunction(CString* name, CString* args, CString* description, aslScriptFunction* asfunc) {
		as_tcl_funcs_s* func = new as_tcl_funcs_s();
		strncpy(func->name, name->c_str(), sizeof(func->name) - 1);
		strncpy(func->args, args->c_str(), sizeof(func->args) - 1);
		strncpy(func->description, description->c_str(), sizeof(func->description) - 1);
		func->asfunc = asfunc;
		as_tcl_funcs.push_back(func);
		g_pScriptSystem->CreateCommand(func->name, func->args, func->description, [](IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv, void* user_args) {
			int index = argv[0]->intValue;
			int menu_index = argv[1]->intValue;
			IPlayerStorageItem* item = g_pEccoFuncs->GetPlayerItem(index);
			IEccoBaseExcutor* menu = g_pEccoFuncs->GetMenuExcutor(menu_index);
			if (!item || !menu)
				return IEccoScriptSystem::Result::Error;
			
			//读取剩下的参数，并将其打包为参数列表传递
			static auto serverManager = ASEXT_GetServerManager();
			static auto as_engine = serverManager->scriptEngine;
			asIScriptContext* ctx = as_engine->RequestContext();
			CTCLArgList* pArgList = new CTCLArgList();
			int remain_argc = argc - 2;
			for(int i = 0; i < remain_argc; i++) {
				CTCLArgItem* arg = new CTCLArgItem();
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
				pArgList->Push(arg);
			}
			auto as_func = reinterpret_cast<asIScriptFunction*>(user_args);
			if (!ctx || ctx->Prepare(as_func) != asSUCCESS) {
				return IEccoScriptSystem::Result::Error;
			}
			ctx->Prepare(as_func);
			ctx->SetArgObject(0, item);
			ctx->SetArgObject(1, menu);
			ctx->SetArgObject(2, pArgList);
			ctx->Execute();
			// 执行并处理返回值
			if (ctx->Execute() == asSUCCESS) {
				byte ret = ctx->GetReturnByte();
				g_pScriptSystem->SetObjectResult(g_pScriptSystem->NewBooleanObject(ret));
			}
			delete pArgList;
			as_engine->ReturnContext(ctx);
			return IEccoScriptSystem::Result::OK;
			}, func->asfunc);
	}
	void UnregisterEccoScriptFunction(CString* name) {
		for (auto it = as_tcl_funcs.begin(); it != as_tcl_funcs.end(); it++) {
			if (strcmp((*it)->name, name->c_str()) == 0) {
				g_pScriptSystem->RemoveCommand((*it)->name);
				delete (*it);
				as_tcl_funcs.erase(it);
				break;
			}
		}
	}

	void GetTranslation(void* pPlayer, CString* code, CString* dest) {
		edict_t* pent = g_pEccoFuncs->PrivateToEdict(pPlayer);
		if (FNullEnt(pent))
			return;
		constexpr size_t TEMP_STR_LEN = 512;
		char temp[TEMP_STR_LEN];
		g_pEccoFuncs->GetTranslation(pent, code->c_str(), temp);
		dest->assign(temp, TEMP_STR_LEN);
	}
};
CEccoFunc g_pASEccoFunc;
void CleanASTCLFuncs() {
	g_pASEccoFunc.CleanTCLFuncs();
}

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
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get player", "CEccoPlayerStorage", "CBasePlayer@ GetPlayer()", IASPlayerStorageItem, GetPlayerAS, asCALL_THISCALL);

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
		ASEXT_RegisterObjectType(pASDoc, "Ecco call arg class", "CTCLArgItem", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get argument type", "CTCLArgItem", "int GetType() const", CTCLArgItem, GetType, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get integer argument", "CTCLArgItem", "int GetInt() const", CTCLArgItem, GetInt, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get string argument", "CTCLArgItem", "void GetString(string&out str)", CTCLArgItem, GetString, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get double argument", "CTCLArgItem", "double GetDouble() const", CTCLArgItem, GetDouble, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get float argument", "CTCLArgItem", "float GetFloat() const", CTCLArgItem, GetFloat, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get boolean argument", "CTCLArgItem", "bool GetBoolean() const", CTCLArgItem, GetBoolean, asCALL_THISCALL);

		ASEXT_RegisterObjectType(pASDoc, "Ecco call arg list", "CTCLArgList", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get size", "CTCLArgList", "uint GetSize() const", CTCLArgList, GetSize, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get argument", "CTCLArgList", "CTCLArgItem@ GetItem(uint index) const", CTCLArgList, GetItem, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get argument", "CTCLArgList", "CTCLArgItem@ opIndex(uint index)", CTCLArgList, GetItem, asCALL_THISCALL);

		ASEXT_RegisterFuncDef(pASDoc, "Ecco menu func callback", "bool fnEccoFuncCallback(CEccoPlayerStorage@ const pPlayer, CEccoMenuExcutor@ const pItem, CTCLArgList@ const pParams)");

		ASEXT_RegisterObjectType(pASDoc, "Ecco functions", "CEccoFuncs", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco storage", "CEccoFuncs", "CEccoPlayerStorage@ GetEccoStorage(CBasePlayer@ pPlayer)", CEccoFunc, GetPlayerStoragePrivate, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco storage", "CEccoFuncs", "CEccoPlayerStorage@ GetEccoStorage(int player_index)", CEccoFunc, GetPlayerStorage, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get Ecco menu", "CEccoFuncs", "CEccoMenuExcutor@ GetMenuByIndex(int menu_index)", CEccoFunc, GetMenuByIndex, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Register Ecco script function", "CEccoFuncs", "void RegisterTCLFunction(string&in name, string&in args, string&in description, fnEccoFuncCallback@ func)", CEccoFunc, RegisterEccoScriptFunction, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Unregister Ecco script function", "CEccoFuncs", "void UnregisteTCLFunction(string&in name)", CEccoFunc, UnregisterEccoScriptFunction, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Get translation", "CEccoFuncs", "void GetTranslation(CBasePlayer@ pPlayer, string&in code, string&out translation)", CEccoFunc, GetTranslation, asCALL_THISCALL);

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
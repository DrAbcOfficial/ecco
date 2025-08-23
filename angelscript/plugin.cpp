#include "api/IEccoPlugin.h"

#include "signatures_template.h"
#include "angelscript/angelscript.hpp"

IMPORT_ASEXT_API_DEFINE()

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

#define PLUGIN_VERSION __DATE__ "T" __TIME__

static void OnPlayerCreditsChanged(IPlayerStorageItem* player, int oldCredits, int &credits) {
    edict_t* plr = player->GetPlayer();
	int nCredits = credits;
    int newCredits = credits;
    AS_CALL(PlayerCreditsChanged, plr->pvPrivateData, oldCredits, nCredits, &newCredits);
	credits = newCredits;
}
static void OnPlayerScoreToCredits(IPlayerStorageItem* player, int score, int& added) {
    edict_t* plr = player->GetPlayer();
    int addedd = added;
    int newAdded = added;
    AS_CALL(PlayerScoreToCredits, plr->pvPrivateData, score, addedd, &newAdded);
    added = newAdded;
}
static void OnMenuExcuted(IEccoBaseExcutor* menu, IPlayerStorageItem* player) {
    edict_t* plr = player->GetPlayer();
    AS_CALL(MenuExcuted, menu, plr->pvPrivateData);
}

class CEccoPlugin : public IEccoPlugin {
	virtual const char* GetName() const { return "AngelScript"; };
	virtual const char* GetDescription() const { return "Angelscript interface"; }
	virtual const char* GetVersion() const { return PLUGIN_VERSION; };
	virtual const char* GetAuthor() const { return "Dr.Abc"; };

	bool ImportASAPI() {
		void* asextHandle = nullptr;
#ifdef _WIN32
		LOAD_PLUGIN(MPLID, "addons/metamod/dlls/asext.dll", PLUG_LOADTIME::PT_ANYTIME, &asextHandle);
#else
		LOAD_PLUGIN(MPLID, "addons/metamod/dlls/asext.so", PLUG_LOADTIME::PT_ANYTIME, &asextHandle);
#endif
		if (!asextHandle)
		{
			LOG_ERROR(MPLID, "asext dll handle not found!");
			return false;
		}
        ASEXT_CallHook = (decltype(ASEXT_CallHook))DLSYM((DLHANDLE)asextHandle, "ASEXT_CallHook");
        if (!ASEXT_CallHook) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CallHook" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_CallCASBaseCallable = (decltype(ASEXT_CallCASBaseCallable))DLSYM((DLHANDLE)asextHandle, "ASEXT_CallCASBaseCallable");
        if (!ASEXT_CallCASBaseCallable) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CallCASBaseCallable" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterDocInitCallback = (decltype(ASEXT_RegisterDocInitCallback))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterDocInitCallback");
        if (!ASEXT_RegisterDocInitCallback) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterDocInitCallback" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterDirInitCallback = (decltype(ASEXT_RegisterDirInitCallback))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterDirInitCallback");
        if (!ASEXT_RegisterDirInitCallback) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterDirInitCallback" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterObjectMethod = (decltype(ASEXT_RegisterObjectMethod))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterObjectMethod");

        if (!ASEXT_RegisterObjectMethod) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterObjectMethod" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterObjectMethodEx = (decltype(ASEXT_RegisterObjectMethodEx))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterObjectMethodEx");

        if (!ASEXT_RegisterObjectMethodEx) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterObjectMethodEx" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterObjectBehaviour = (decltype(ASEXT_RegisterObjectBehaviour))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterObjectBehaviour");
        if (!ASEXT_RegisterObjectBehaviour) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterObjectBehaviour" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterObjectBehaviourEx = (decltype(ASEXT_RegisterObjectBehaviourEx))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterObjectBehaviourEx");
        if (!ASEXT_RegisterObjectBehaviourEx) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterObjectBehaviourEx" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterObjectType = (decltype(ASEXT_RegisterObjectType))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterObjectType");
        if (!ASEXT_RegisterObjectType) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterObjectType" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterObjectProperty = (decltype(ASEXT_RegisterObjectProperty))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterObjectProperty");
        if (!ASEXT_RegisterObjectProperty) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterObjectProperty" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterGlobalProperty = (decltype(ASEXT_RegisterGlobalProperty))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterGlobalProperty");
        if (!ASEXT_RegisterGlobalProperty) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterGlobalProperty" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterHook = (decltype(ASEXT_RegisterHook))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterHook");
        if (!ASEXT_RegisterHook) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterHook" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterFuncDef = (decltype(ASEXT_RegisterFuncDef))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterFuncDef");
        if (!ASEXT_RegisterFuncDef) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterFuncDef" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterEnum = (decltype(ASEXT_RegisterEnum))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterEnum");
        if (!ASEXT_RegisterEnum) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterEnum" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_RegisterEnumValue = (decltype(ASEXT_RegisterEnumValue))DLSYM((DLHANDLE)asextHandle, "ASEXT_RegisterEnumValue");
        if (!ASEXT_RegisterEnumValue) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_RegisterEnumValue" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_CreateDirectory = (decltype(ASEXT_CreateDirectory))DLSYM((DLHANDLE)asextHandle, "ASEXT_CreateDirectory");
        if (!ASEXT_CreateDirectory) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CreateDirectory" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_CStringAssign = (decltype(ASEXT_CStringAssign))DLSYM((DLHANDLE)asextHandle, "ASEXT_CStringAssign");
        if (!ASEXT_CStringAssign) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CStringAssign" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_CStringdtor = (decltype(ASEXT_CStringdtor))DLSYM((DLHANDLE)asextHandle, "ASEXT_CStringdtor");

        if (!ASEXT_CStringdtor) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CStringdtor" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_GetServerManager = (decltype(ASEXT_GetServerManager))DLSYM((DLHANDLE)asextHandle, "ASEXT_GetServerManager");
        if (!ASEXT_GetServerManager) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_GetServerManager" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_CreateCASFunction = (decltype(ASEXT_CreateCASFunction))DLSYM((DLHANDLE)asextHandle, "ASEXT_CreateCASFunction");
        if (!ASEXT_CreateCASFunction) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CreateCASFunction" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_CASRefCountedBaseClass_InternalRelease = (decltype(ASEXT_CASRefCountedBaseClass_InternalRelease))DLSYM((DLHANDLE)asextHandle, "ASEXT_CASRefCountedBaseClass_InternalRelease");
        if (!ASEXT_CASRefCountedBaseClass_InternalRelease) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CASRefCountedBaseClass_InternalRelease" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_CScriptAny_Release = (decltype(ASEXT_CScriptAny_Release))DLSYM((DLHANDLE)asextHandle, "ASEXT_CScriptAny_Release");
        if (!ASEXT_CScriptAny_Release) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CScriptAny_Release" " from " "asext" " dll !");
            return 0;
        }
        ASEXT_CScriptArray_Release = (decltype(ASEXT_CScriptArray_Release))DLSYM((DLHANDLE)asextHandle, "ASEXT_CScriptArray_Release");
        if (!ASEXT_CScriptArray_Release) {
            LOG_ERROR(MPLID, "Failed to get " "ASEXT_CScriptArray_Release" " from " "asext" " dll !");
            return 0;
        }
		return true;
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
		if (ImportASAPI()) {
			RegisterAngelScriptMethods();
			RegisterAngelScriptHooks();
		}
	};
	virtual void Initialize(){
        g_pEccoFuncs->AddHook(Hook_Type::ON_PLAYER_CREDITS_CHANGED, &OnPlayerCreditsChanged);
		g_pEccoFuncs->AddHook(Hook_Type::ON_PLAYER_SCORE_TO_CREDITS, &OnPlayerScoreToCredits);
		g_pEccoFuncs->AddHook(Hook_Type::ON_MENU_EXCUTE, &OnMenuExcuted);
    };
	virtual void Shutdown() {};
};
static CEccoPlugin s_EccoPluginInstance;

ECCO_API IEccoPlugin* GetEccoPluginInstance() {
	return &s_EccoPluginInstance;
}
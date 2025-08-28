/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */
#include <extdll.h>
#include <meta_api.h>

#include "scripts/script_system.h"
#include "menu/MenuParser.h"
#include "storage/Storage.h"

#include "Scheduler.h"
#include "timer/Timer.h"
#include "hud/hud.h"
#include "lang/lang.h"
#include "command/CEccoBaseCommand.h"
#include "command/command_system.h"
#include "plugin/plugin_system.h"
#include "config/CConfig.h"

#include "dlldef.h"

#pragma region PreHooks

static void GameInit() {
	InitHud();
	ResetTranslations();
	LoadTranslations();
	RegistServerCommandToEngine();
	CallPluginInit();
	SET_META_RESULT(MRES_HANDLED);
}

static void ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {
	SET_META_RESULT(MRES_HANDLED);
	ResetHud();
	ResetEccoScriptItems();
	CleanPlayerLastCredits(nullptr);

	const char* mapname = STRING(gpGlobals->mapname);
	bool is_banned_map = IsBannedMap(mapname);
	if (is_banned_map)
		return;

	LoadEccoScriptItems();
	ReseAllMenus();
	ParseRootMenu();
	PrecacheAllScriptItems();
}

bool g_bIsSeriesMap = false;
static void ServerDeactivate() {
	extern bool g_bLevelChangedOccurred;
	g_bIsSeriesMap = g_bLevelChangedOccurred;
	g_bLevelChangedOccurred = false;

	RemoveAllPlayerStorage();
	SET_META_RESULT(MRES_HANDLED);
}

static void	 ClientCommand (edict_t* pEntity) {
	const char* cmd = CMD_ARGV(0);
	if (!strncmp(cmd, "say", 3))
		RETURN_META(ClientSayCommandHandler(pEntity) ? MRES_SUPERCEDE : MRES_IGNORED);
	if (!StringNCaseCmp(cmd, ECCO_CMD_PREFIX, ECCO_CMD_PREFIX_LEN))
		SET_META_RESULT(ClientCommandHandler(pEntity) ? MRES_SUPERCEDE : MRES_IGNORED);
	else
		SET_META_RESULT(TextMenuClientCommandHook(pEntity) ? MRES_SUPERCEDE : MRES_IGNORED);
}

static void ClientDisconnect(edict_t* pEntity) {
	RemoveHudUpdateTimer(pEntity);
	RemovePlayerScoreToCreditsTimer(pEntity);
	StorageClientDisconnectHandle(pEntity);
	SET_META_RESULT(MRES_HANDLED);
}

constexpr int QUERY_CVAR_CONST = 114514;
static void ClientPutInServer(edict_t* pEntity) {
	StorageClientPutinServerHandle(pEntity);
	AddPlayerScoreToCreditsTimer(pEntity);
	AddHudUpdateTimer(pEntity);
	auto config = GetEccoConfig();
	if (config->QueryLangCVar.size() > 0)
		QUERY_CLIENT_CVAR_VALUE2(pEntity, config->QueryLangCVar.c_str(), ENTINDEX(pEntity) + QUERY_CVAR_CONST);
	SET_META_RESULT(MRES_HANDLED);
}

static void StartFrame() {
	g_Scheduler.Think();
	SET_META_RESULT(MRES_HANDLED);
}
static DLL_FUNCTIONS gFunctionTable = {
	GameInit,					// pfnGameInit
	NULL,					// pfnSpawn
	NULL,					// pfnThink
	NULL,					// pfnUse
	NULL,				// pfnTouch
	NULL,				// pfnBlocked
	NULL,					// pfnKeyValue
	NULL,					// pfnSave
	NULL,					// pfnRestore
	NULL,					// pfnSetAbsBox

	NULL,					// pfnSaveWriteFields
	NULL,					// pfnSaveReadFields

	NULL,					// pfnSaveGlobalState
	NULL,					// pfnRestoreGlobalState
	NULL,					// pfnResetGlobalState

	NULL,					// pfnClientConnect
	ClientDisconnect,					// pfnClientDisconnect
	NULL,					// pfnClientKill
	ClientPutInServer,					// pfnClientPutInServer
	ClientCommand,					// pfnClientCommand
	NULL,					// pfnClientUserInfoChanged
	ServerActivate,					// pfnServerActivate
	ServerDeactivate,					// pfnServerDeactivate

	NULL,					// pfnPlayerPreThink
	NULL,					// pfnPlayerPostThink

	StartFrame,					// pfnStartFrame
	NULL,					// pfnParmsNewLevel
	NULL,					// pfnParmsChangeLevel

	NULL,					// pfnGetGameDescription
	NULL,					// pfnPlayerCustomization

	NULL,					// pfnSpectatorConnect
	NULL,					// pfnSpectatorDisconnect
	NULL,					// pfnSpectatorThink

	NULL,					// pfnSys_Error

	NULL,					// pfnPM_Move
	NULL,					// pfnPM_Init
	NULL,					// pfnPM_FindTextureType

	NULL,					// pfnSetupVisibility
	NULL,					// pfnUpdateClientData
	NULL,					// pfnAddToFullPack
	NULL,					// pfnCreateBaseline
	NULL,					// pfnRegisterEncoders
	NULL,					// pfnGetWeaponData
	NULL,					// pfnCmdStart
	NULL,					// pfnCmdEnd
	NULL,					// pfnConnectionlessPacket
	NULL,					// pfnGetHullBounds
	NULL,					// pfnCreateInstancedBaselines
	NULL,					// pfnInconsistentFile
	NULL,					// pfnAllowLagCompensation
};
C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS* pFunctionTable,
	int* interfaceVersion) {
	if (!pFunctionTable) {
		UTIL_LogPrintf("GetEntityAPI2 called with null pFunctionTable");
		return(FALSE);
	}
	else if (*interfaceVersion != INTERFACE_VERSION) {
		UTIL_LogPrintf("GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
		//! Tell metamod what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
	return(TRUE);
}
#pragma endregion

#pragma region PostHook
static DLL_FUNCTIONS gFunctionTable_Post = {
	NULL,					// pfnGameInit
	NULL,					// pfnSpawn
	NULL,					// pfnThink
	NULL,					// pfnUse
	NULL,				// pfnTouch
	NULL,				// pfnBlocked
	NULL,					// pfnKeyValue
	NULL,					// pfnSave
	NULL,					// pfnRestore
	NULL,					// pfnSetAbsBox

	NULL,					// pfnSaveWriteFields
	NULL,					// pfnSaveReadFields

	NULL,					// pfnSaveGlobalState
	NULL,					// pfnRestoreGlobalState
	NULL,					// pfnResetGlobalState

	NULL,					// pfnClientConnect
	NULL,					// pfnClientDisconnect
	NULL,					// pfnClientKill
	NULL,					// pfnClientPutInServer
	NULL,					// pfnClientCommand
	NULL,					// pfnClientUserInfoChanged
	NULL,					// pfnServerActivate
	NULL,					// pfnServerDeactivate

	NULL,					// pfnPlayerPreThink
	NULL,					// pfnPlayerPostThink

	NULL,					// pfnStartFrame
	NULL,					// pfnParmsNewLevel
	NULL,					// pfnParmsChangeLevel

	NULL,					// pfnGetGameDescription
	NULL,					// pfnPlayerCustomization

	NULL,					// pfnSpectatorConnect
	NULL,					// pfnSpectatorDisconnect
	NULL,					// pfnSpectatorThink

	NULL,					// pfnSys_Error

	NULL,					// pfnPM_Move
	NULL,					// pfnPM_Init
	NULL,					// pfnPM_FindTextureType

	NULL,					// pfnSetupVisibility
	NULL,					// pfnUpdateClientData
	NULL,					// pfnAddToFullPack
	NULL,					// pfnCreateBaseline
	NULL,					// pfnRegisterEncoders
	NULL,					// pfnGetWeaponData
	NULL,					// pfnCmdStart
	NULL,					// pfnCmdEnd
	NULL,					// pfnConnectionlessPacket
	NULL,					// pfnGetHullBounds
	NULL,					// pfnCreateInstancedBaselines
	NULL,					// pfnInconsistentFile
	NULL,					// pfnAllowLagCompensation
};
C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS* pFunctionTable,
	int* interfaceVersion) {
	if (!pFunctionTable) {
		UTIL_LogPrintf("GetEntityAPI2 called with null pFunctionTable");
		return(FALSE);
	}
	else if (*interfaceVersion != INTERFACE_VERSION) {
		UTIL_LogPrintf("GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
		//! Tell metamod what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return(FALSE);
	}
	memcpy(pFunctionTable, &gFunctionTable_Post, sizeof(DLL_FUNCTIONS));
	return(TRUE);
}
#pragma endregion

#pragma region NewDllFunctions

static void CvarValue2(const edict_t* pEnt, int requestID, const char* cvarName, const char* value){
	if (!strcmp(value, "Bad CVAR request") || !strcmp(value, "Bad Player")) {
		RETURN_META(MRES_IGNORED);
	}
	if(requestID > QUERY_CVAR_CONST) {
		int index = requestID - QUERY_CVAR_CONST;
		edict_t* pent = INDEXENT(index);
		if (FNullEnt(pent)) {
			RETURN_META(MRES_IGNORED);
		}
		auto item = GetPlayerStorageItem(pent);
		item->SetLang(value);
		item->SaveData();
		RETURN_META(MRES_HANDLED);
	}
	SET_META_RESULT(MRES_IGNORED);
}

static NEW_DLL_FUNCTIONS gNewDllFunctionTable =
{
	// Called right before the object's memory is freed. 
	// Calls its destructor.
	NULL,
	NULL,
	NULL,

	// Added 2005/08/11 (no SDK update):
	NULL,//void(*pfnCvarValue)(const edict_t *pEnt, const char *value);

	// Added 2005/11/21 (no SDK update):
	//    value is "Bad CVAR request" on failure (i.e that user is not connected or the cvar does not exist).
	//    value is "Bad Player" if invalid player edict.
	CvarValue2,//void(*pfnCvarValue2)(const edict_t *pEnt, int requestID, const char *cvarName, const char *value);
};

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS* pNewDllFunctionTable,
	int* interfaceVersion)
{
	if (!pNewDllFunctionTable) {
		LOG_ERROR(PLID, "GetNewDLLFunctions called with null pFunctionTable");
		return(FALSE);
	}
	else if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
		LOG_ERROR(PLID, "GetNewDLLFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, NEW_DLL_FUNCTIONS_VERSION);
		//! Tell metamod what version we had, so it can figure out who is out of date.
		*interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return(FALSE);
	}
	memcpy(pNewDllFunctionTable, &gNewDllFunctionTable, sizeof(NEW_DLL_FUNCTIONS));

	return(TRUE);
}
#pragma endregion

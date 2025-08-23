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
	ResetHud();
	ResetEccoScriptItems();
	CleanPlayerLastCredits(nullptr);

	const char* mapname = STRING(gpGlobals->mapname);
	bool is_banned_map = IsBannedMap(mapname);
	extern bool g_bIsSeriesMap;
	if (is_banned_map) {
		LoadEccoScriptItems();
		ReseAllMenus();
		ParseRootMenu();
		PrecacheAllScriptItems();

		
		int save_set = GetEccoConfig()->StorePlayerScore;
		if (save_set < 2) {
			if (save_set <= 0)
				CleanPlayerCredites(nullptr);
			else if (save_set == 1) {
				if (!g_bIsSeriesMap)
					CleanPlayerCredites(nullptr);
			}
		}
	}
	g_bIsSeriesMap = false;
	SET_META_RESULT(MRES_HANDLED);
}

static void	 ClientCommand (edict_t* pEntity) {
	const char* cmd = CMD_ARGV(0);
	if (!strncmp(cmd, "say", 3))
		RETURN_META(ClientSayCommandHandler(pEntity) ? MRES_SUPERCEDE : MRES_IGNORED);
	if (!_strnicmp(cmd, ECCO_CMD_PREFIX, ECCO_CMD_PREFIX_LEN))
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
static void ClientPutInServer(edict_t* pEntity) {
	StorageClientPutinServerHandle(pEntity);
	AddPlayerScoreToCreditsTimer(pEntity);
	AddHudUpdateTimer(pEntity);
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
	NULL,					// pfnServerDeactivate

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
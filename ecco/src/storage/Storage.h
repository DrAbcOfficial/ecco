#pragma once

#include "CPlayerStorageItem.h"

/// <summary>
/// Reset player credits to start, if pent is null reset all players
/// </summary>
/// <param name="pent">target</param>
extern void CleanPlayerCredites(edict_t* pent);
extern void CleanPlayerLastCredits(edict_t* pent);
extern void StorageClientPutinServerHandle(edict_t* pent);
extern void StorageClientDisconnectHandle(edict_t* pent);
extern CPlayerStorageItem* GetPlayerStorageItem(edict_t* pent);
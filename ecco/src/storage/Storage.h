#pragma once

#include "CPlayerStorageItem.h"

extern void RemoveAllPlayerStorage();
extern void CleanPlayerLastCredits(edict_t* pent);
extern void StorageClientPutinServerHandle(edict_t* pent);
extern void StorageClientDisconnectHandle(edict_t* pent);
extern CPlayerStorageItem* GetPlayerStorageItem(edict_t* pent);
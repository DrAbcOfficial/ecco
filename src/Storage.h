#pragma once

#include "CPlayerStorageItem.h"

extern void StorageClientPutinServerHandle(edict_t* pent);
extern void StorageClientDisconnectHandle(edict_t* pent);
extern CPlayerStorageItem* GetPlayerStorageItem(edict_t* pent);
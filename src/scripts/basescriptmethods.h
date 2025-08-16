#pragma once
#include <string>
#include <extdll.h>

#include "storage/CPlayerStorageItem.h"

namespace EccoBaseScriptMethods {
	extern void give(CPlayerStorageItem* player, std::string name);
	extern void saytext(CPlayerStorageItem* player, std::string content);
	extern void saytextall(std::string content);
	extern void addmoney(CPlayerStorageItem* player, int money);
	extern void setmoney(CPlayerStorageItem* player, int money);
	extern int getmoney(CPlayerStorageItem* player);
	extern void maxhealth(CPlayerStorageItem* player, int value);
	extern void maxarmor(CPlayerStorageItem* player, int value);
	extern void maxspeed(CPlayerStorageItem* player, int value);
	extern void gravity(CPlayerStorageItem* player, int value);
}
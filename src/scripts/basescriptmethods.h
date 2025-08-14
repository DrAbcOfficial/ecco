#pragma once
#include <string>
#include <extdll.h>

#include "storage/CPlayerStorageItem.h"

namespace EccoBaseScriptMethods {
	extern void give(CPlayerStorageItem* player, std::string name);
	extern void saytext(CPlayerStorageItem* player, std::string content);
	extern void saytextall(std::string content);
}
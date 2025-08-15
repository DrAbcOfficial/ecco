#include "CEccoClientCommand.h"
#include "menu/Menuparser.h"

static CEccoClientCommand buy("buy", "open buy menu", [](edict_t* caller, const std::vector<std::string>& args) -> bool {
	g_pRootMenuExecutor->Excute(caller, 0);
	return true;
});
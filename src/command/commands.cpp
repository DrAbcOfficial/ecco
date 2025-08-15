#include "CEccoClientCommand.h"
#include "CEccoServerCommand.h"
#include "menu/Menuparser.h"
#include "scripts/script_system.h"
#include "storage/Storage.h"

#include "meta_utility.h"
#include <charconv>

#pragma region Client
static CEccoClientCommand buy("buy", "open buy menu", ADMIN_LEVEL::NONE, [](edict_t* caller, const std::vector<std::string>& args) -> bool {
	g_pRootMenuExecutor->Excute(caller, 0);
	return true;
	});
static CEccoClientCommand reset("reload", "reload all script", ADMIN_LEVEL::ADMIN, [](edict_t* caller, const std::vector<std::string>& args) -> bool {
	ResetEccoScriptItems();
	LoadEccoScriptItems();
	ReseAllMenus();
	ParseRootMenu();
	return true;
});
#pragma endregion

#pragma region Server
static CEccoServerCommand setadmin("set_admin", "set a player as admin", { CEccoCmdArgSet("SteamId"), CEccoCmdArgSet("AdminLevel")}, [](const std::vector<std::string>& args)->bool {
    auto str_to_adminlevel = [](const std::string& s) -> short {
        if (s.empty())
            return 0;
        size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
            ++start;
        }
        if (start == s.size())
            return 0;
        short value = 0;
        const char* first = s.data() + start;
        const char* last = s.data() + s.size();
        auto [ptr, ec] = std::from_chars(first, last, value);
        if (ec == std::errc() && ptr == last)
            return value;
        else
            return 0;
    };

    short result = str_to_adminlevel(args[1]);
    ADMIN_LEVEL level = result < 0 ? ADMIN_LEVEL::NONE : (result > (short)ADMIN_LEVEL::OWNER ? ADMIN_LEVEL::OWNER : static_cast<ADMIN_LEVEL>(result));
    const char* id = args[0].c_str();
	edict_t* pent = GetPlayerBySteamId(id);
    if (pent) {
        auto item = GetPlayerStorageItem(pent);
        item->SetAdminLevel(level);
        return true;
    }
    else
        LOG_CONSOLE(PLID, "Set ADMIN_LEVEL %d to %s, but Player is NULL!", result, id);
    return false;
});
#pragma endregion
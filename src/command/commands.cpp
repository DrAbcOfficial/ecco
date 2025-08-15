#include <unordered_map>
#include <format>

#include "CEccoClientCommand.h"
#include "CEccoServerCommand.h"
#include "menu/Menuparser.h"
#include "scripts/script_system.h"
#include "storage/Storage.h"

#include "meta_utility.h"

#pragma region Client
static CEccoClientCommand buy("buy", "open buy menu", ADMIN_LEVEL::NONE, [](edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args) -> bool {
	g_pRootMenuExecutor->Excute(caller, 0);
	return true;
	});
static CEccoClientCommand reset("reload", "reload all script", ADMIN_LEVEL::ADMIN, [](edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args) -> bool {
    pThis->PrintTranslatedMessageByFrom(caller, talk, "ecco_scripts_reloading");
    ReseAllMenus();
	ResetEccoScriptItems();
	LoadEccoScriptItems();
	ParseRootMenu();
    pThis->PrintTranslatedMessageByFrom(caller, talk, "ecco_scripts_reloaded");
	return true;
});
constexpr char HELP_FORMAT[] = "|{:<12}|{:<24}|{:<48}|";
extern std::unordered_map<std::string, CEccoClientCommand*> s_mapRegistedClientCmdMap;
static CEccoClientCommand help("help", "list all commands", ADMIN_LEVEL::NONE, [](edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args) -> bool {
    std::string buffer = std::format(HELP_FORMAT, "Command", "Description", "Usage");
    pThis->PrintMessageByFrom(caller, talk, buffer.c_str());
    for (auto& cmd : s_mapRegistedClientCmdMap) {
        auto& item = cmd.second;
        buffer = std::format(HELP_FORMAT, item->m_szCmd, item->m_szDescription, item->GetUsage());
        pThis->PrintMessageByFrom(caller, talk, buffer.c_str());
    }
    return true;
});
#pragma endregion

#pragma region Server
static CEccoServerCommand setadmin("set_admin", "set a player as admin", { CEccoCmdArgSet("SteamId64"), CEccoCmdArgSet("AdminLevel")}, [](CEccoServerCommand* pThis, const std::vector<std::string>& args)->bool {
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
extern std::unordered_map<std::string, CEccoServerCommand*> s_mapRegistedServerCmdMap;
static CEccoServerCommand help_s("help", "list all commands", [](CEccoServerCommand* pThis, const std::vector<std::string>& args) -> bool {
    std::string buffer = std::format(HELP_FORMAT, "Command", "Description", "Usage");
    LOG_CONSOLE(PLID, buffer.c_str());
    for (auto& cmd : s_mapRegistedServerCmdMap) {
        auto& item = cmd.second;
        buffer = std::format(HELP_FORMAT, item->m_szCmd, item->m_szDescription, item->GetUsage());
        LOG_CONSOLE(PLID, buffer.c_str());
    }
    return true;
});
#pragma endregion
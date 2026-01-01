#include <unordered_map>
#include <format>

#include "CEccoClientCommand.h"
#include "CEccoServerCommand.h"
#include "menu/MenuParser.h"
#include "scripts/script_system.h"
#include "storage/Storage.h"
#include "lang/lang.h"
#include "api/IEccoPlugin.h"
#include "config/CConfig.h"

#include "meta_utility.h"

using namespace EccoMetaUtility;

#pragma region Client
static CEccoClientCommand buy("buy", "open buy menu", ADMIN_LEVEL::NONE, { CEccoCmdArgSet("pages", true) }, [](edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args) -> bool {
    const char* mapname = STRING(gpGlobals->mapname);
    bool is_banned_map = IsBannedMap(mapname);
    if (is_banned_map) {
		pThis->PrintTranslatedMessageByFrom(caller, talk, "ecco_banned_map");
        return false;
    }
	bool is_alive = caller->v.deadflag == DEAD_NO;
    if (!is_alive && !GetEccoConfig()->BuyMenu.AllowDeathPlayerBuy) {
        pThis->PrintTranslatedMessageByFrom(caller, talk, "ecco_dead_player_buy");
        return false;
    }
    if (args.size() > 0) {
        CEccoTextMenuExecutor* pExecutor = g_pRootMenuExecutor;
        for (auto& s : args) {
            int input = StringToInteger(s) - 1;
            size_t chose = static_cast<size_t>(input < 0 ? 9 : input);
            if (chose >= MAX_MENU_OPTIONS)
                return false;
            auto script = dynamic_cast<CEccoScriptExecutor*>(pExecutor->GetOption(chose));
            if (script) {
                script->Excute(caller, 0);
                return true;
            }
            pExecutor = dynamic_cast<CEccoTextMenuExecutor*>(pExecutor->GetOption(chose));
            if (!pExecutor)
                return false;
        }
        if (pExecutor != nullptr && pExecutor != g_pRootMenuExecutor) {
            pExecutor->Excute(caller, 0);
            return true;
        }
        return false;
    }
    else {
        g_pRootMenuExecutor->Excute(caller, 0);
        return true;
    }  
});
static CEccoClientCommand reload("reload", "reload all script", ADMIN_LEVEL::ADMIN, [](edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args) -> bool {
    pThis->PrintTranslatedMessageByFrom(caller, talk, "ecco_scripts_reloading");
    ResetAllMenus();
	ResetEccoScriptItems();
    ResetTranslations();
    LoadTranslations();
	LoadEccoScriptItems();
	ParseRootMenu();
    pThis->PrintTranslatedMessageByFrom(caller, talk, "ecco_scripts_reloaded");
	return true;
});
static CEccoClientCommand setlang("lang", "set language", ADMIN_LEVEL::NONE, { CEccoCmdArgSet("language", true) }, [](edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args) -> bool {
    if (args.size() == 0) {
        auto item = GetPlayerStorageItem(caller);
        pThis->PrintMessageByFrom(caller, talk, item->GetLang());
        return true;
    }
    auto& target_lang = args[0];
    auto& langs = GetAvaliableLangs();
    if (target_lang.empty() || std::find(langs.begin(), langs.end(), target_lang) == langs.end()) {
        pThis->PrintTranslatedMessageByFrom(caller, talk, "ecco_lang_invalid");
        std::string buffer = "";
        for (auto& l : langs) {
            buffer += l + " ";
        }
        pThis->PrintMessageByFrom(caller, talk, buffer.c_str());
        return false;
	}
    auto item = GetPlayerStorageItem(caller);
    item->SetLang(target_lang.c_str());
    pThis->PrintTranslatedMessageByFrom(caller, talk, "ecco_lang_success");
    return true;
});
constexpr char HELP_FORMAT[] = "|{:<12}|{:<24}|{:<48}|";
extern std::unordered_map<std::string, CEccoClientCommand*> s_mapRegistedClientCmdMap;
static CEccoClientCommand help("help", "list all commands", ADMIN_LEVEL::NONE, [](edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args) -> bool {
    std::string buffer = std::format(HELP_FORMAT, "Command", "Description", "Usage");
    pThis->PrintMessageByFrom(caller, talk, buffer.c_str());
    auto storage = GetPlayerStorageItem(caller);
    for (auto& cmd : s_mapRegistedClientCmdMap) {
        auto& item = cmd.second;
        if (item->GetAdminLevel() > storage->GetAdminLevel())
            continue;
        buffer = std::format(HELP_FORMAT, item->m_szCmd, item->m_szDescription, item->GetUsage());
        pThis->PrintMessageByFrom(caller, talk, buffer.c_str());
    }
    return true;
});
#pragma endregion

#pragma region Server
static CEccoServerCommand setadmin("set_admin", "set a player as admin", { CEccoCmdArgSet("SteamId64"), CEccoCmdArgSet("AdminLevel")}, [](CEccoServerCommand* pThis, const std::vector<std::string>& args)->bool {

    short result = StringToInteger(args[1]);
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
constexpr char PLUGIN_FORMAT[] = "|{:<8}|{:<8}|{:<24}|{:<24}|";
static CEccoServerCommand list_plugins("listplugins", "list all plugins", [](CEccoServerCommand* pThis, const std::vector<std::string>& args) -> bool {
    static std::vector<std::string> buffers;
    if (buffers.size() == 0) {
        buffers.push_back(std::format(PLUGIN_FORMAT, "Name", "Author", "Version", "Description"));
        extern std::vector<IEccoPlugin*> g_aryPlugins;
        for (auto& ext : g_aryPlugins) {
            buffers.push_back(std::format(PLUGIN_FORMAT, ext->GetName(), ext->GetAuthor(), ext->GetVersion(), ext->GetDescription()));
        }
    }
    for (auto& str : buffers) {
        LOG_CONSOLE(PLID, str.c_str());
    }
    return true;
});
static CEccoServerCommand dump_tcldocs("dump_tcldocs", "dump all tcl documents", [](CEccoServerCommand* pThis, const std::vector<std::string>& args) -> bool {
    extern std::vector<std::string> GetTCLCommandsDoc();
	auto buffers = GetTCLCommandsDoc();
    for (auto& str : buffers) {
        LOG_CONSOLE(PLID, str.c_str());
    }
    return true;
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

static CEccoServerCommand setconfig("setconfig", "set or get config value", { CEccoCmdArgSet("path", true), CEccoCmdArgSet("value", false) }, [](CEccoServerCommand* pThis, const std::vector<std::string>& args) -> bool {
    if (args.size() == 0) {
        LOG_CONSOLE(PLID, "Usage: setconfig <path> [value]");
        LOG_CONSOLE(PLID, "  setconfig <path> - get config value");
        LOG_CONSOLE(PLID, "  setconfig <path> <value> - set config value");
        LOG_CONSOLE(PLID, "  setconfig list - list all config paths");
        return false;
    }

    if (args[0] == "list") {
        auto paths = GetAllConfigPaths();
        LOG_CONSOLE(PLID, "Available config paths:");
        for (const auto& path : paths) {
            LOG_CONSOLE(PLID, "  %s", path.c_str());
        }
        return true;
    }

    const std::string& path = args[0];
    if (args.size() == 1) {
        std::string value = GetConfigValue(path);
        if (value.empty()) {
            LOG_CONSOLE(PLID, "Config path '%s' not found", path.c_str());
            return false;
        }
        LOG_CONSOLE(PLID, "%s = %s", path.c_str(), value.c_str());
        return true;
    }

    const std::string& value = args[1];
    if (SetConfigValue(path, value)) {
        LOG_CONSOLE(PLID, "Config '%s' set to '%s'", path.c_str(), value.c_str());
        return true;
    } else {
        LOG_CONSOLE(PLID, "Failed to set config '%s' to '%s'", path.c_str(), value.c_str());
        return false;
    }
});

static CEccoServerCommand saveconfig("saveconfig", "save config to file", [](CEccoServerCommand* pThis, const std::vector<std::string>& args) -> bool {
    if (SaveEccoConfig()) {
        LOG_CONSOLE(PLID, "Config saved successfully");
        return true;
    } else {
        LOG_CONSOLE(PLID, "Failed to save config");
        return false;
    }
});
#pragma endregion
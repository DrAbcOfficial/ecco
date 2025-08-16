#include <unordered_map>

#include "CEccoServerCommand.h"
#include <meta_api.h>
#include "meta_utility.h"

std::unordered_map<std::string, CEccoServerCommand*> s_mapRegistedServerCmdMap;

static void EngineCallWrapper() {
    std::string cmd = CMD_ARGV(0);
    auto it = s_mapRegistedServerCmdMap.find(cmd.substr(ECCO_CMD_PREFIX_LEN));
    if (it != s_mapRegistedServerCmdMap.end())
        it->second->Call();
}

void RegistServerCommandToEngine() {
    for (auto& cmd : s_mapRegistedServerCmdMap) {
        std::string temp = ECCO_CMD_PREFIX + cmd.first;
        (*g_engfuncs.pfnAddServerCommand)(const_cast<char*>(temp.c_str()), &EngineCallWrapper);
    }
}

CEccoServerCommand::CEccoServerCommand(const char* cmd, const char* description,
    std::function<bool(CEccoServerCommand* pThis, const std::vector<std::string>& args)> callback) {
    m_szCmd = cmd;
    m_szDescription = description;
    m_pfnCallback = std::move(callback);
    s_mapRegistedServerCmdMap.emplace(cmd, this);
}
CEccoServerCommand::CEccoServerCommand(const char* cmd, const char* description, std::vector<CEccoCmdArgSet> arglist,
    std::function<bool(CEccoServerCommand* pThis, const std::vector<std::string>& args)> callback) {
    m_szCmd = cmd;
    m_szDescription = description;
    m_aryArgList = std::move(arglist);
    m_pfnCallback = std::move(callback);
    s_mapRegistedServerCmdMap.emplace(cmd, this);
}

bool CEccoServerCommand::Call(){
    int argc = CMD_ARGC();
    std::vector<std::string> args{};
    for (int i = 1; i < argc; i++) {
        args.push_back(CMD_ARGV(i));
    }
    return PrivateCall(args);
}

bool CEccoServerCommand::PrivateCall(const std::vector<std::string>& args){
    if (!m_pfnCallback)
        return false;
    if (CheckArgs(args)) {
        std::string errorMsg = m_szDescription;
        errorMsg += "\n    Usage: \n    " + GetUsage();
        LOG_CONSOLE(PLID, errorMsg.c_str());
        return true;
    }
    return m_pfnCallback(this, args);
}

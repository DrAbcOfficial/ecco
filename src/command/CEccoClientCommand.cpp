#include <map>
#include <algorithm>

#include "CEccoClientCommand.h"
#include "storage/Storage.h"
#include "lang/lang.h"
#include "config/CConfig.h"
#include "menu/MenuParser.h"

#include <meta_api.h>
#include "meta_utility.h"

static std::map<std::string, CEccoClientCommand*> s_mapRegistedCommandMap;

bool ClientCommandHandler(edict_t* caller) {
    if (!caller || !IsValidPlayer(caller)) 
        return false;
    int argc = CMD_ARGC();
    if (argc < 1) 
        return false;
    std::string cmd = CMD_ARGV(0);
    auto it = s_mapRegistedCommandMap.find(cmd.substr(5));
    if (it == s_mapRegistedCommandMap.end()) 
        return false;
    CEccoClientCommand* command = it->second;
    if (!command) 
        return false;
	return command->Call(caller, false);
}

bool ClientSayCommandHandler(edict_t* caller) {
    if (!caller || !IsValidPlayer(caller))
        return false;
    int argc = CMD_ARGC();
    if (argc <= 1)
        return false;
    std::string cmd = CMD_ARGV(1);
    if (cmd[0] == '!' || cmd[0] == '/') {
        size_t space_pos = cmd.find_first_of(' ');
        std::string command;
        std::string args;
        if (space_pos != std::string::npos) {
            command = cmd.substr(0, space_pos);
            args = cmd.substr(space_pos);
        }
        else {
            command = cmd;
            args = "";
        }
        auto& trigger = GetEccoConfig()->BuyMenu.OpenShopTriggers;
        auto it = std::find(trigger.begin(), trigger.end(), command.substr(1));
        if (it != trigger.end()) {
            g_pRootMenuExecutor->Excute(caller, 0);
            return true;
        }
    }
    return false;
}

CEccoClientCommand::CEccoClientCommand(const char* cmd, const char* description, ADMIN_LEVEL prv, 
    std::function<bool(edict_t* caller, const std::vector<std::string>& args)> callback){
	m_szCmd = cmd;
	m_szDescription = description;
    m_iPrivilege = prv;
	m_pfnCallback = std::move(callback);
	s_mapRegistedCommandMap.emplace(cmd, this);
}

CEccoClientCommand::CEccoClientCommand(const char* cmd, const char* description, ADMIN_LEVEL prv,
    std::vector<CEccoCmdArgSet> arglist, std::function<bool(edict_t* caller, const std::vector<std::string>& args)> callback){
	m_aryArgList = std::move(arglist);
    m_szCmd = cmd;
    m_szDescription = description;
    m_iPrivilege = prv;
    m_pfnCallback = std::move(callback);
    s_mapRegistedCommandMap.emplace(cmd, this);
}

bool CEccoClientCommand::Call(edict_t* caller, bool from_talk){
    int argc = CMD_ARGC();
    std::vector<std::string> args{};
    for (int i = 1; i < argc; i++) {
        args.push_back(CMD_ARGV(i));
    }
	return PrivateCall(caller, from_talk, args);
}

bool CEccoClientCommand::PrivateCall(edict_t* caller, bool from_talk, const std::vector<std::string>& args){
    if (!m_pfnCallback)
        return false;
    if(CheckArgs(args)){
		std::string errorMsg = m_szDescription;
        errorMsg += "\n    Usage: \n    " + m_szCmd + " ";
        for (const auto& arg_set : m_aryArgList) {
            if (!arg_set.m_bIsOptional)
                errorMsg += "[" + arg_set.m_szName + "] ";
            else
				errorMsg += "<" + arg_set.m_szName + "> ";
		}
        PrintMessageByFrom(caller, from_talk, errorMsg.c_str());
        return true;
	}
    auto item = GetPlayerStorageItem(caller);
    if (!item)
        return false;
    if (item->GetAdminLevel() < m_iPrivilege) {
        PrintMessageByFrom(caller, from_talk, GetTranslation(caller, "ecco_cmd_access_deny").c_str());
        return true;
    }
    return m_pfnCallback(caller, args);
}

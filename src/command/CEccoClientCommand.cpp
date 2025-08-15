#include <map>

#include "CEccoClientCommand.h"

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

CEccoClientCommand::CEccoClientCommand(const char* cmd, const char* description, std::function<bool(edict_t* caller, const std::vector<std::string>& args)> callback){
	m_szCmd = cmd;
	m_szDescription = description;
	m_pfnCallback = std::move(callback);
	s_mapRegistedCommandMap.emplace(cmd, this);
}

CEccoClientCommand::CEccoClientCommand(const char* cmd, const char* description, std::vector<CEccoClientCommandArgSet> arglist, std::function<bool(edict_t* caller, const std::vector<std::string>& args)> callback){
	m_aryArgList = std::move(arglist);
    m_szCmd = cmd;
    m_szDescription = description;
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

void CEccoClientCommand::PrintMessageByFrom(edict_t* caller, bool from_talk, const char* message){
    ClientPrintf(caller, from_talk ? ClientPrintTarget::Talk : ClientPrintTarget::Console, message);
}

bool CEccoClientCommand::PrivateCall(edict_t* caller, bool from_talk, const std::vector<std::string>& args){
    if (!m_pfnCallback)
        return false;
    size_t neededArgs = 0;
    for (const auto& arg_set : m_aryArgList) {
        if (!arg_set.m_bIsOptional)
            neededArgs++;
    }
    if(neededArgs > args.size()){
		std::string errorMsg = m_szDescription;
        errorMsg += "    Usage: " + m_szCmd + " ";
        for (const auto& arg_set : m_aryArgList) {
            if (!arg_set.m_bIsOptional)
                errorMsg += "[" + arg_set.m_szName + "] ";
            else
				errorMsg += "<" + arg_set.m_szName + "> ";
		}
        PrintMessageByFrom(caller, from_talk, errorMsg.c_str());
        return false;
	}
    return m_pfnCallback(caller, args);
}

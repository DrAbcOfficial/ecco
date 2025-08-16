#include "CEccoBaseCommand.h"

#include "lang/lang.h"

#include <meta_api.h>
#include "meta_utility.h"

bool CEccoBaseCommand::CheckArgs(const std::vector<std::string>& args){
    size_t neededArgs = 0;
    for (const auto& arg_set : m_aryArgList) {
        if (!arg_set.m_bIsOptional)
            neededArgs++;
    }
    return neededArgs > args.size();
}

std::string CEccoBaseCommand::GetUsage(){
    std::string buffer = ECCO_CMD_PREFIX + m_szCmd + " ";
    for (const auto& arg_set : m_aryArgList) {
        if (!arg_set.m_bIsOptional)
            buffer += "[" + arg_set.m_szName + "] ";
        else
            buffer += "<" + arg_set.m_szName + "> ";
    }
    return buffer;
}

void CEccoBaseCommand::PrintTranslatedMessageByFrom(edict_t* caller, bool from_talk, const char* key){
    std::string translated = GetTranslation(caller, key);
    PrintMessageByFrom(caller, from_talk, translated.c_str());
}

void CEccoBaseCommand::PrintMessageByFrom(edict_t* caller, bool from_talk, const char* message) {
    ClientPrintf(caller, from_talk ? ClientPrintTarget::Talk : ClientPrintTarget::Console, message);
}

CEccoCmdArgSet::CEccoCmdArgSet(const char* name, bool optional){
    m_szName = name;
    m_bIsOptional = optional;
}

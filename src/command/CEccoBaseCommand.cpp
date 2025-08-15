#include "CEccoBaseCommand.h"

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

void CEccoBaseCommand::PrintMessageByFrom(edict_t* caller, bool from_talk, const char* message) {
    ClientPrintf(caller, from_talk ? ClientPrintTarget::Talk : ClientPrintTarget::Console, message);
}

CEccoCmdArgSet::CEccoCmdArgSet(const char* name, bool optional){
    m_szName = name;
    m_bIsOptional = optional;
}

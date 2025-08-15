#pragma once
#include "CEccoBaseCommand.h"
#include "storage/CPlayerStorageItem.h"

class CEccoClientCommand : public CEccoBaseCommand {
public:
	CEccoClientCommand(const char* cmd, const char* description, ADMIN_LEVEL prv, 
		std::function<bool(edict_t* caller, const std::vector<std::string>& args)> callback);
	CEccoClientCommand(const char* cmd, const char* description, ADMIN_LEVEL prv, std::vector<CEccoCmdArgSet> arglist,
		std::function<bool(edict_t* caller, const std::vector<std::string>& args)> callback);
	bool Call(edict_t* caller, bool from_talk);
	template <typename... Strings>
	bool Call(edict_t* caller, bool from_talk, Strings&&... args){
		std::vector<std::string> args_list = { std::forward<Strings>(args)... };
		return PrivateCall(caller, from_talk, args_list);
	}
private:
	std::function<bool(edict_t* caller, const std::vector<std::string>& args)> m_pfnCallback;
	ADMIN_LEVEL m_iPrivilege;
	bool PrivateCall(edict_t* caller, bool from_talk, const std::vector<std::string>& args);
};


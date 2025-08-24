#pragma once
#include "CEccoBaseCommand.h"
#include "storage/CPlayerStorageItem.h"

class CEccoClientCommand : public CEccoBaseCommand {
public:
	CEccoClientCommand(const char* cmd, const char* description, ADMIN_LEVEL prv, 
		std::function<bool(edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args)> callback);
	CEccoClientCommand(const char* cmd, const char* description, ADMIN_LEVEL prv, std::vector<CEccoCmdArgSet> arglist,
		std::function<bool(edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args)> callback);
	bool Call(edict_t* caller, bool from_talk);
	template <typename... Strings>
	bool Call(edict_t* caller, bool from_talk, Strings&&... args){
		std::vector<std::string> args_list = { std::forward<Strings>(args)... };
		return DirectCall(caller, from_talk, args_list);
	}

	bool DirectCall(edict_t* caller, bool from_talk, const std::vector<std::string>& args);

	ADMIN_LEVEL GetAdminLevel() const;
private:
	std::function<bool(edict_t* caller, CEccoClientCommand* pThis, bool talk, const std::vector<std::string>& args)> m_pfnCallback;
	ADMIN_LEVEL m_iPrivilege;
};


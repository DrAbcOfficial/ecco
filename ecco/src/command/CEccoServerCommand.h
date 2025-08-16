#pragma once
#include "CEccoBaseCommand.h"
class CEccoServerCommand : public CEccoBaseCommand{
public:
	CEccoServerCommand(const char* cmd, const char* description,
		std::function<bool(CEccoServerCommand* pThis, const std::vector<std::string>& args)> callback);
	CEccoServerCommand(const char* cmd, const char* description, std::vector<CEccoCmdArgSet> arglist,
		std::function<bool(CEccoServerCommand* pThis, const std::vector<std::string>& args)> callback);
	bool Call();
	template <typename... Strings>
	bool Call(Strings&&... args) {
		std::vector<std::string> args_list = { std::forward<Strings>(args)... };
		return PrivateCall(args_list);
	}
private:
	std::function<bool(CEccoServerCommand* pThis, const std::vector<std::string>& args)> m_pfnCallback;
	bool PrivateCall(const std::vector<std::string>& args);
};


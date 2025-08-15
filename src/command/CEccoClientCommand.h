#pragma once
#include <functional>
#include <string>
#include <vector>
#include <extdll.h>

class CEccoClientCommandArgSet {
public:
	std::string m_szName;
	bool m_bIsOptional;
};

class CEccoClientCommand{
public:
	CEccoClientCommand(const char* cmd, const char* description, std::function<bool(edict_t* caller, const std::vector<std::string>& args)> callback);
	CEccoClientCommand(const char* cmd, const char* description, std::vector<CEccoClientCommandArgSet> arglist, std::function<bool(edict_t* caller, const std::vector<std::string>& args)> callback);
	std::string m_szCmd;
	std::string m_szDescription;

	bool Call(edict_t* caller, bool from_talk);
	template <typename... Strings>
	bool Call(edict_t* caller, Strings&&... args, bool from_talk){
		std::vector<std::string> args_list = { std::forward<Strings>(args)... };
		return PrivateCall(caller, from_talk, args_list);
	}
	static void PrintMessageByFrom(edict_t* caller, bool from_talk, const char* message);
private:
	std::vector<CEccoClientCommandArgSet> m_aryArgList;
	std::function<bool(edict_t* caller, const std::vector<std::string>& args)> m_pfnCallback;

	bool PrivateCall(edict_t* caller, bool from_talk, const std::vector<std::string>& args);
};


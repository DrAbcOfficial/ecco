#pragma once
#include <functional>
#include <string>
#include <vector>
#include <extdll.h>

constexpr char ECCO_CMD_PREFIX[] = "ecco_";
constexpr size_t ECCO_CMD_PREFIX_LEN = std::char_traits<char>::length(ECCO_CMD_PREFIX);


class CEccoCmdArgSet {
public:
	CEccoCmdArgSet(const char* name, bool optional = false);
	std::string m_szName;
	bool m_bIsOptional;
};

class CEccoBaseCommand
{
public:
	std::string m_szCmd;
	std::string m_szDescription;

	bool CheckArgs(const std::vector<std::string>& args);
	std::string GetUsage();

	static void PrintMessageByFrom(edict_t* caller, bool from_talk, const char* message);
protected:
	std::vector<CEccoCmdArgSet> m_aryArgList;
};
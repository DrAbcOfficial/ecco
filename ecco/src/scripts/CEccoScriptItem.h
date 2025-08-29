#pragma once
#include <string>
#include <vector>
#include <unordered_map>
class CEccoScriptItem{
public:
	//原始未拆分的Name
	std::string m_szName;
	//拆分后的Id
	std::string m_szId;
	int m_iFlags; // 32-bit flags for various options
	int m_iCost;
	std::string m_szScripts;

	CEccoScriptItem(std::string path);
	~CEccoScriptItem();

	void Precache();

private:
	std::vector<char*> m_aryPrecaches{};
	std::vector<char*> m_arySoundPrecaches{};
	std::vector<char*> m_aryOtherPrecaches{};
};
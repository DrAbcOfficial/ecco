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

	std::vector<std::string> m_aryPrecaches;
	std::vector<std::string> m_arySoundPrecaches;

	CEccoScriptItem(std::string path);
};
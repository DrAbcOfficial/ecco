#pragma once
#include <string>
#include <vector>
#include <unordered_map>
class CEccoScriptItem{
public:
	//ԭʼδ��ֵ�Name
	std::string m_szName;
	//��ֺ��Id
	std::string m_szId;
	int m_iCost;
	std::string m_szScripts;

	std::vector<std::string> m_aryPrecaches;

	CEccoScriptItem(std::string path);
};


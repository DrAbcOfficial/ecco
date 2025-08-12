#pragma once
#include <unordered_map>
#include <string>
#include "sven_api.hpp"

enum class MenuItemFlag {
	NONE = 0,
	HIDECOST = 1 << 0
};

class CEccoMenuItem {
public:
	CEccoMenuItem(char* script_content);
	void Excute();

	//Id
	std::string m_szDisplay;
	int8_t m_iPage;
	MenuItemFlag m_iFlags = MenuItemFlag::NONE;
	
private:
	CEccoMenuItem* m_pParent;
	std::unordered_map<std::string, CEccoMenuItem*> m_aryChild;
	IEccoExcuter* m_pExcuter;
};
#include <ranges>

#include "MenuParser.h"
#include "script_system.h"

#include "CEccoScriptExecutor.h"
#include "CEccoBackExecutor.h"

using ecco_parser_item_t = struct ecco_parser_item_s {
	std::string szId;
	ecco_parser_item_s* pParent;
	std::vector<ecco_parser_item_s*> aryChild;
	CEccoScriptItem* pScriptItem = nullptr;
};

static std::vector<ecco_parser_item_t*> s_aryPentToDelete;

static void ParseItem(ecco_parser_item_s* pParent, CEccoScriptItem* pItem, std::string& name) {
	size_t pos = name.find('.');
	if (pos == std::string::npos) {
		ecco_parser_item_t* pParserItem = new ecco_parser_item_t();
		pParserItem->szId = name;
		pParserItem->pScriptItem = pItem;
		pParserItem->pParent = pParent;
		pParent->aryChild.push_back(pParserItem);
		s_aryPentToDelete.push_back(pParserItem);
	}
	else {
		std::string id = name.substr(0, pos);
		std::string next = name.substr(pos + 1);
		auto it = std::ranges::find_if(
			pParent->aryChild,
			[&id](const ecco_parser_item_s* item) { return item->szId == id; }
		);
		if (it != pParent->aryChild.end())
			ParseItem(*it, pItem, next);
		else {
			ecco_parser_item_t* pParserItem = new ecco_parser_item_t();
			pParserItem->szId = id;
			pParserItem->pParent = pParent;
			pParent->aryChild.push_back(pParserItem);
			s_aryPentToDelete.push_back(pParserItem);
			ParseItem(pParserItem, pItem, next);
		}
	}
}

static void ParseMenu(CEccoTextMenuExecutor* pParentExecutor, ecco_parser_item_t* pParserItem) {
	int counter = 0;
	int page = 0;
	CEccoTextMenuExecutor* pMenu = pParentExecutor;
	for (auto& child : pParserItem->aryChild) {
		if (page > 0 && counter == 0) {
			//下一页
			CEccoTextMenuExecutor* pNextPageMenu = new CEccoTextMenuExecutor();
			pNextPageMenu->m_szId = "ecco_menu_next_page";
			pNextPageMenu->m_pParent = pMenu;
			pMenu->AddItem(pNextPageMenu);
			pMenu = pNextPageMenu;
		}
		if (child->pScriptItem) {
			CEccoScriptExecutor* pExecutor = new CEccoScriptExecutor();
			pExecutor->m_szId = pParserItem->pScriptItem->m_szId;
			pExecutor->m_iCost = pParserItem->pScriptItem->m_iCost;
			pExecutor->m_szScript = pParserItem->pScriptItem->m_szScripts;
			pExecutor->m_bitFlags = pParserItem->pScriptItem->m_iFlags;
			pExecutor->m_pParent = pMenu;
			pMenu->AddItem(pExecutor);
		}
		else {
			CEccoTextMenuExecutor* pExecutor = new CEccoTextMenuExecutor();
			pExecutor->m_szId = pParserItem->szId;
			pExecutor->m_pParent = pMenu;
			pMenu->AddItem(pExecutor);
			ParseMenu(pExecutor, child);
		}
		counter++;
		if (counter % 6 == 0) {
			//返回
			CEccoBackExecutor* pBackExecutor = new CEccoBackExecutor();
			pBackExecutor->m_szId = "ecco_menu_back";
			pBackExecutor->m_pParentExecutor = pParentExecutor;
			pMenu->AddItem(pBackExecutor);
			//上一页
			if (page > 0) {
				CEccoBackExecutor* pPrevPageExecutor = new CEccoBackExecutor();
				pPrevPageExecutor->m_szId = "ecco_menu_prev_page";
				pPrevPageExecutor->m_pParentExecutor = pMenu;
				pMenu->AddItem(pPrevPageExecutor);
			}
			page++;
			counter = 0;
		}
	}
}

CEccoTextMenuExecutor* ParseMenu(){
	//构建初级树
	ecco_parser_item_t* pRootParserItem = new ecco_parser_item_t();
	s_aryPentToDelete.push_back(pRootParserItem);
	for(auto iter = g_aryEccoScriptItems.begin(); iter != g_aryEccoScriptItems.end(); iter++){
		CEccoScriptItem* pItem = *iter;
		if (pItem->m_szId.empty() || pItem->m_szName.empty()) {
			LOG_CONSOLE(PLID, "Ecco script item %s has no id or name, skipping", pItem->m_szName.c_str());
			continue;
		}
		ParseItem(pRootParserItem, pItem, pItem->m_szName);
	}
	//构建菜单执行器
	CEccoTextMenuExecutor* pRootExecutor = new CEccoTextMenuExecutor();
	pRootExecutor->m_szId = "ecco_menu_root";
	ParseMenu(pRootExecutor, pRootParserItem);
	return pRootExecutor;
}

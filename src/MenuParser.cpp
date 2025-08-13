#include <ranges>

#include "MenuParser.h"
#include "script_system.h"

#include "CEccoScriptExcutor.h"

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

static void ParseMenu(CEccoTextMenuExcutor* pParentExecutor, ecco_parser_item_t* pParserItem) {
	if (pParserItem->aryChild.size() > MAX_MENU_OPTIONS) {
		//处理六个
		for (size_t i = 0; i < 6; i++) {
			CEccoTextMenuExcutor* pExecutor = new CEccoTextMenuExcutor();
			pExecutor->m_szId = pParserItem->szId;
			pParentExecutor->AddItem(pExecutor);
			auto iter = pParserItem->aryChild.begin();
			ParseMenu(pExecutor, *iter);
			pParserItem->aryChild.erase(iter);
		}
		//添加返回上级，上一页，下一页，关闭选项
		//返回上级

		//上一页

		//下一页
		
		//关闭选项
	}
	else {

	}
	for(auto& child : pParserItem->aryChild) {
		if (child->pScriptItem) {
			CEccoScriptExcutor* pExecutor = new CEccoScriptExcutor();
			pExecutor->m_szId = pParserItem->pScriptItem->m_szId;
			pExecutor->m_iCost = pParserItem->pScriptItem->m_iCost;
			pExecutor->m_szScript = pParserItem->pScriptItem->m_szScripts;
			pExecutor->m_bitFlags = pParserItem->pScriptItem->m_iFlags;
			pParentExecutor->AddItem(pExecutor);
		}
		else {
			CEccoTextMenuExcutor* pExecutor = new CEccoTextMenuExcutor();
			
			ParseMenu(pExecutor, child);
		}
	}
}

CEccoTextMenuExcutor* ParseMenu(){
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
	CEccoTextMenuExcutor* pRootExecutor = new CEccoTextMenuExcutor();

	return nullptr;
}

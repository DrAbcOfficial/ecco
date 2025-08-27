#include <ranges>
#include <algorithm>

#include "scripts/script_system.h"

#include "menu/MenuParser.h"
#include "menu/executor/CEccoScriptExecutor.h"
#include "menu/executor/CEccoBackExecutor.h"

CEccoTextMenuExecutor* g_pRootMenuExecutor = nullptr;
using ecco_parser_item_t = struct ecco_parser_item_s {
	std::string szId;
	ecco_parser_item_s* pParent = nullptr;
	std::vector<ecco_parser_item_s*> aryChild;
	CEccoScriptItem* pScriptItem = nullptr;
};

static std::vector<ecco_parser_item_t*> s_aryPentToDelete;
std::vector<CBaseEccoExecutor*> g_aryEccoMenuExecutors;

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

static void ParseMenu(CEccoTextMenuExecutor* pParentExecutor, ecco_parser_item_t* pParserItem, bool root = false) {
	int counter = 0;
	int page = 0;

	auto add_back = [&](CEccoTextMenuExecutor* pMenu) {
		//返回
		if (root) {
			pMenu->AddItem(nullptr);
			return;
		}
		CEccoBackExecutor* pBackExecutor = new CEccoBackExecutor();
		pBackExecutor->m_iIndex = g_aryEccoMenuExecutors.size();
		g_aryEccoMenuExecutors.push_back(pBackExecutor);
		pBackExecutor->m_szId = "ecco_menu_back";
		pBackExecutor->m_pParentExecutor = pParentExecutor->m_pParent;
		pMenu->AddItem(pBackExecutor);
	};
	auto add_prev = [&](CEccoTextMenuExecutor* pMenu) {
		//上一页
		if (page > 0) {
			CEccoBackExecutor* pPrevPageExecutor = new CEccoBackExecutor();
			pPrevPageExecutor->m_iIndex = g_aryEccoMenuExecutors.size();
			g_aryEccoMenuExecutors.push_back(pPrevPageExecutor);
			pPrevPageExecutor->m_szId = "ecco_menu_prev_page";
			pPrevPageExecutor->m_pParentExecutor = pMenu->m_pParent;
			pMenu->AddItem(pPrevPageExecutor);
		}
		else
			pMenu->AddItem(nullptr);
	};
	auto add_exit = [](CEccoTextMenuExecutor* pMenu) {
		//关闭
		CBaseEccoExecutor* pExitExecutor = new CBaseEccoExecutor();
		pExitExecutor->m_szId = "ecco_menu_exit";
		pMenu->AddItem(pExitExecutor);
	};
	CEccoTextMenuExecutor* pMenu = pParentExecutor;
	std::sort(pParserItem->aryChild.begin(), pParserItem->aryChild.end(), [](ecco_parser_item_t* a, ecco_parser_item_t* b) {
		return a->szId < b->szId;
	});
	for (auto& child : pParserItem->aryChild) {
		if (page > 0 && counter == 0) {
			//下一页
			CEccoTextMenuExecutor* pNextPageMenu = new CEccoTextMenuExecutor();
			pNextPageMenu->m_iIndex = g_aryEccoMenuExecutors.size();
			g_aryEccoMenuExecutors.push_back(pNextPageMenu);
			pNextPageMenu->m_szTitle = pMenu->m_szTitle;
			pNextPageMenu->m_szId = "ecco_menu_next_page";
			pNextPageMenu->m_pParent = pMenu;
			pMenu->AddItem(pNextPageMenu);
			//关闭
			add_exit(pMenu);
			pMenu = pNextPageMenu;
		}
		if (child->pScriptItem) {
			CEccoScriptExecutor* pExecutor = new CEccoScriptExecutor();
			pExecutor->m_iIndex = g_aryEccoMenuExecutors.size();
			g_aryEccoMenuExecutors.push_back(pExecutor);
			pExecutor->m_szId = child->pScriptItem->m_szId;
			pExecutor->m_iCost = child->pScriptItem->m_iCost;
			pExecutor->m_szScript = child->pScriptItem->m_szScripts;
			pExecutor->m_bitFlags = child->pScriptItem->m_iFlags;
			pExecutor->m_pParent = pMenu;
			pMenu->AddItem(pExecutor);
		}
		else {
			CEccoTextMenuExecutor* pExecutor = new CEccoTextMenuExecutor();
			pExecutor->m_iIndex = g_aryEccoMenuExecutors.size();
			g_aryEccoMenuExecutors.push_back(pExecutor);
			pExecutor->m_szId = child->szId;
			pExecutor->m_szTitle = child->szId;
			pExecutor->m_pParent = pMenu;
			pMenu->AddItem(pExecutor);
			ParseMenu(pExecutor, child);
		}
		counter++;
		if (pParserItem->aryChild.size() > MAX_ITEM_NO_PAGE && counter % (MAX_ITEMS_PER_PAGE - 1) == 0) {
			add_back(pMenu);
			add_prev(pMenu);
			page++;
			counter = 0;
		}
	}
	int result = pParserItem->aryChild.size() % (MAX_ITEMS_PER_PAGE - 1);
	if (page > 0) {
		if (result != 0) {
			//填充空白
			for (int i = result; i < MAX_ITEMS_PER_PAGE - 1; i++) {
				pMenu->AddItem(nullptr);
			}
			//返回和上一页
			add_back(pMenu);
			add_prev(pMenu);
			pMenu->AddItem(nullptr);
			//关闭
			add_exit(pMenu);
		}
	}
	else {
		for (int i = pMenu->GetSize(); i <= MAX_ITEMS_PER_PAGE; i++) {
			pMenu->AddItem(nullptr);
		}
		if (pParserItem->aryChild.size() <= (MAX_ITEMS_PER_PAGE + 1))
			add_back(pMenu);
		add_exit(pMenu);
	}
}

void ReseAllMenus() {
	extern std::array<CEccoTextMenuExecutor*, MAX_PLAYERS> g_aryTextMenus;
	for (size_t i = 0; i < g_aryTextMenus.size(); i++) {
		g_aryTextMenus[i] = nullptr;
	}
	for(auto& executor : g_aryEccoMenuExecutors) {
		delete executor;
	}
	g_aryEccoMenuExecutors.clear();
}

void ParseRootMenu(){
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
	pRootExecutor->m_iIndex = g_aryEccoMenuExecutors.size();
	g_aryEccoMenuExecutors.push_back(pRootExecutor);
	pRootExecutor->m_szId = "ecco_menu_root";
	ParseMenu(pRootExecutor, pRootParserItem, true);
	//清理临时数据
	for (auto& pent : s_aryPentToDelete) {
		delete pent;
	}
	s_aryPentToDelete.clear();
	//设置全局根菜单执行器
	g_pRootMenuExecutor = pRootExecutor;
}
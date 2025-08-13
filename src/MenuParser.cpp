#include <ranges>

#include "MenuParser.h"
#include "script_system.h"

#include "CEccoScriptExecutor.h"
#include "CEccoBackExecutor.h"

CEccoTextMenuExecutor* g_pRootMenuExecutor = nullptr;
std::vector<CBaseEccoExecutor*> g_aryEccoMenuExecutors;

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

	auto add_back_prev = [&](CEccoTextMenuExecutor* pMenu) {
		//����
		CEccoBackExecutor* pBackExecutor = new CEccoBackExecutor();
		g_aryEccoMenuExecutors.push_back(pBackExecutor);
		pBackExecutor->m_szId = "ecco_menu_back";
		pBackExecutor->m_pParentExecutor = pParentExecutor;
		pMenu->AddItem(pBackExecutor);
		//��һҳ
		if (page > 0) {
			CEccoBackExecutor* pPrevPageExecutor = new CEccoBackExecutor();
			g_aryEccoMenuExecutors.push_back(pPrevPageExecutor);
			pPrevPageExecutor->m_szId = "ecco_menu_prev_page";
			pPrevPageExecutor->m_pParentExecutor = pMenu;
			pMenu->AddItem(pPrevPageExecutor);
		}
		else
			pMenu->AddItem(nullptr);
	};
	auto add_exit = [](CEccoTextMenuExecutor* pMenu) {
		//�ر�
		CBaseEccoExecutor* pExitExecutor = new CBaseEccoExecutor();
		pExitExecutor->m_szId = "ecco_menu_exit";
		pMenu->AddItem(pExitExecutor);
	};
	CEccoTextMenuExecutor* pMenu = pParentExecutor;
	for (auto& child : pParserItem->aryChild) {
		if (page > 0 && counter == 0) {
			//��һҳ
			CEccoTextMenuExecutor* pNextPageMenu = new CEccoTextMenuExecutor();
			g_aryEccoMenuExecutors.push_back(pNextPageMenu);
			pNextPageMenu->m_szId = "ecco_menu_next_page";
			pNextPageMenu->m_pParent = pMenu;
			pMenu->AddItem(pNextPageMenu);
			//�ر�
			add_exit(pMenu);
			pMenu = pNextPageMenu;
		}
		if (child->pScriptItem) {
			CEccoScriptExecutor* pExecutor = new CEccoScriptExecutor();
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
			g_aryEccoMenuExecutors.push_back(pExecutor);
			pExecutor->m_szId = child->szId;
			pExecutor->m_pParent = pMenu;
			pMenu->AddItem(pExecutor);
			ParseMenu(pExecutor, child);
		}
		counter++;
		if (counter % 6 == 0) {
			add_back_prev(pMenu);
			page++;
			counter = 0;
		}
	}
	int result = pParserItem->aryChild.size() % 6;
	if (page > 0 && result != 0) {
		//���հ�
		for(int i = result; i < 6; i++) {
			pMenu->AddItem(nullptr);
		}
		//���غ���һҳ
		add_back_prev(pMenu);
		pMenu->AddItem(nullptr);
		//�ر�
		add_exit(pMenu);
	}
}

void ReseAllMenus() {
	for(auto& executor : g_aryEccoMenuExecutors) {
		delete executor;
	}
	g_aryEccoMenuExecutors.clear();
}

void ParseRootMenu(){
	//����������
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
	//�����˵�ִ����
	CEccoTextMenuExecutor* pRootExecutor = new CEccoTextMenuExecutor();
	g_aryEccoMenuExecutors.push_back(pRootExecutor);
	pRootExecutor->m_szId = "ecco_menu_root";
	ParseMenu(pRootExecutor, pRootParserItem);
	//������ʱ����
	for (auto& pent : s_aryPentToDelete) {
		delete pent;
	}
	s_aryPentToDelete.clear();
	//����ȫ�ָ��˵�ִ����
	g_pRootMenuExecutor = pRootExecutor;
}

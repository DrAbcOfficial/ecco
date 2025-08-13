#include "MenuParser.h"


using ecco_parser_item_t = struct ecco_parser_item_s {
	ecco_parser_item_s* pParent;
	std::vector<ecco_parser_item_s*> aryChild;
	bool bTerminal;
	std::string szId;
	int iCost;
	std::string szScript;
};

CEccoTextMenuExcutor* ParseMenu()
{
	std::vector<ecco_parser_item_t*> pentToDelete;
	ecco_parser_item_t* pRootParserItem = new ecco_parser_item_t();
	pentToDelete.push_back(pRootParserItem);

	return nullptr;
}

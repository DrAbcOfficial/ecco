#pragma once
#include <vector>
#include <string>

#include "CEccoTextMenu.h"

using ecco_parser_item = struct{
	ecco_parser_item* pParent;
	std::vector<ecco_parser_item*> aryChild;
	bool bTerminal;
	std::string szId;
	int iCost;
	std::string szScript;
};

extern CEccoTextMenu* ParseMenu();
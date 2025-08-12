// Most are copied from wootguy's mmlib

#include "meta_utility.h"
#include "CEccoMenu.h"

CEccoMenu* g_aryTextMenus[MAX_PLAYERS];

// listen for any other functions/plugins opening menus, so that TextMenu knows if it's the active menu
void TextMenuMessageBeginHook(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed) {
	if (msg_type != MSG_ShowMenu)
		return;
	for (int i = 0; i < MAX_PLAYERS; i++) {
		g_aryTextMenus[i]->HandleMenuMessage(msg_dest, ed);
	}
}

// handle player selections
bool TextMenuClientCommandHook(edict_t* pEntity) {
	if (!_strcmpi(CMD_ARGV(0), "menuselect")) {
		int selection = atoi(CMD_ARGV(1)) - 1;
		if (selection < 0 || selection >= MAX_MENU_OPTIONS)
			return true;
		for (int i = 0; i < MAX_PLAYERS; i++) {
			g_aryTextMenus[i]->HandleMenuselectCmd(pEntity, selection);
		}
		return true;
	}
	return false;
}


void CEccoMenu::HandleMenuMessage(int msg_dest, edict_t* ed) {

	// Another text menu has been opened for one or more players, so this menu
	// is no longer visible and should not handle menuselect commands

	// If this message is in fact triggered by this object, then the viewer flags should be set
	// after this func finishes
	if (!m_iViewers)
		return;

	if ((msg_dest == MSG_ONE || msg_dest == MSG_ONE_UNRELIABLE) && ed)
		m_iViewers &= ~(PLAYER_BIT(ed));
	else if (msg_dest == MSG_ALL || msg_dest == MSG_ALL) 
		m_iViewers = 0;
}

void CEccoMenu::HandleMenuselectCmd(edict_t* pEntity, int selection) {
	if (!m_iViewers)
		return;
	int playerbit = PLAYER_BIT(pEntity);
	if (m_iViewers & playerbit) {
		if (selection == 9) {
			// exit menu
		}
		else if (IsPaginated() && selection == 7) {
			Excute(pEntity, m_iLastPage - 1, false);
		}
		else if (IsPaginated() && selection == 8) {
			Excute(pEntity, m_iLastPage + 1, false);
		}
		else if (selection < GetOptionSize() && IsValidPlayer(pEntity)) {
			//Callbackhere
			//Get CEccoMenuItem and excute it
		}
	}
}

inline int CEccoMenu::GetOptionSize(){
	return m_aryOption.size();
}

bool CEccoMenu::IsPaginated() {
	return GetOptionSize() > 9;
}

void CEccoMenu::SetTitle(const char* title) {
	m_szTitle = title;
}

void CEccoMenu::AddItem(CEccoMenuItem* pItem) {
	if (GetOptionSize() >= MAX_MENU_OPTIONS) {
		//META_LOG("Maximum menu options reached! Failed to add: %s", pItem->m_szDisplay.c_str());
		return;
	}
	m_aryOption.push_back(pItem);
}

void CEccoMenu::Excute(edict_t* pPlayer, int8_t iPage, bool bReOpen) {
	std::string buffer = m_szTitle + "\n\n";
	uint16_t validSlots = (1 << (EXITOPTION_INDEX - 1)); // exit option always valid

	m_iLastPage = iPage;
	int limitPerPage = IsPaginated() ? ITEMS_PER_PAGE : 9;
	int itemOffset = iPage * ITEMS_PER_PAGE;
	int totalPages = (GetOptionSize() + 6) / ITEMS_PER_PAGE;
	int addedOptions = 0;

	for (int i = itemOffset, k = 0; i < itemOffset + limitPerPage && i < GetOptionSize(); i++, k++) {
		buffer += std::to_string(k + 1) + ": " + m_aryOption[i]->m_szDisplay + "\n";
		validSlots |= (1 << k);
		addedOptions++;
	}

	while (IsPaginated() && addedOptions < ITEMS_PER_PAGE) {
		buffer += "\n";
		addedOptions++;
	}

	buffer += "\n";

	// TODO: Change this thing with translation
	if (IsPaginated()) {
		if (iPage > 0) {
			buffer += "8: Back\n";
			validSlots |= (1 << 7);
		}
		else {
			buffer += "\n";
		}
		if (iPage < totalPages - 1) {
			buffer += "9: More\n";
			validSlots |= (1 << 8);
		}
		else {
			buffer += "\n";
		}
	}
	buffer += std::to_string(EXITOPTION_INDEX % 10) + ": Exit";

	if (IsValidPlayer(pPlayer)) {
		MESSAGE_BEGIN(MSG_ONE, MSG_ShowMenu, NULL, pPlayer);
		WRITE_SHORT(validSlots);
		WRITE_CHAR(m_iDuration);
		WRITE_BYTE(FALSE); // "need more" (?)
		WRITE_STRING(buffer.c_str());
		MESSAGE_END();

		m_iViewers |= PLAYER_BIT(pPlayer);
	}
	else {
		MESSAGE_BEGIN(MSG_ALL, MSG_ShowMenu);
		WRITE_SHORT(validSlots);
		WRITE_CHAR(m_iDuration);
		WRITE_BYTE(FALSE); // "need more" (?)
		WRITE_STRING(buffer.c_str());
		MESSAGE_END();

		m_iViewers = 0xffffffff;
	}
}
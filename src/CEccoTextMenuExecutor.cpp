// Shit, wootguy didnt finish TextMenu
// Have to write by myself :(
#include <array>

#include "meta_utility.h"
#include "CEccoTextMenuExecutor.h"

std::array<CEccoTextMenuExecutor*, MAX_PLAYERS> g_aryTextMenus;

// listen for any other functions/plugins opening menus, so that TextMenu knows if it's the active menu
void TextMenuMessageBeginHook(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed) {
	if (msg_type != MSG_ShowMenu)
		return;
	if ((msg_dest == MSG_ONE || msg_dest == MSG_ONE_UNRELIABLE) && IsValidPlayer(ed)) {
		int index = ENTINDEX(ed) - 1;
		auto& item = g_aryTextMenus[index];
		if (item != nullptr)
			item->HandleMenuMessage(msg_dest, ed);
	}
	else if (msg_dest == MSG_ALL) {
		for (auto iter = g_aryTextMenus.begin(); iter != g_aryTextMenus.end(); iter++) {
			auto& item = *iter;
			if(item != nullptr)
				item->HandleMenuMessage(msg_dest, ed);
		}
	}
}

// handle player selections
bool TextMenuClientCommandHook(edict_t* pEntity) {
	if (!_stricmp(CMD_ARGV(0), "menuselect")) {
		int selection = atoi(CMD_ARGV(1)) - 1;
		if (selection < 0 || selection >= MAX_MENU_OPTIONS)
			return true;
		int index = ENTINDEX(pEntity) - 1;
		auto& item = g_aryTextMenus[index];
		if (item != nullptr)
			item->HandleMenuselectCmd(pEntity, selection);
		return true;
	}
	return false;
}


bool CEccoTextMenuExecutor::IsPlayerViewing(edict_t* ent){
	if (!IsValidPlayer(ent))
		return false;
	int index = ENTINDEX(ent)-1;
	return m_iViewers.test(index);
}

void CEccoTextMenuExecutor::SetPlayerViewing(edict_t* ent, bool view){
	int index = ENTINDEX(ent) - 1;
	m_iViewers.set(index, view);
	g_aryTextMenus[index] = view ? this : nullptr;
}

void CEccoTextMenuExecutor::HandleMenuMessage(int msg_dest, edict_t* ed) {
	// Another text menu has been opened for one or more players, so this menu
	// is no longer visible and should not handle menuselect commands

	// If this message is in fact triggered by this object, then the viewer flags should be set
	// after this func finishes
	if (m_iViewers.none())
		return;

	if ((msg_dest == MSG_ONE || msg_dest == MSG_ONE_UNRELIABLE) && ed)
		SetPlayerViewing(ed, false);
	else if (msg_dest == MSG_ALL) {
		m_iViewers.reset();
		for (size_t i = 0; i < g_aryTextMenus.size(); i++) {
			g_aryTextMenus[i] = nullptr;
		}
	}
	else
		LOG_MESSAGE(PLID, "Who the hell use msg_dest: %d in ShowMenu message????", msg_dest);
}

void CEccoTextMenuExecutor::HandleMenuselectCmd(edict_t* pEntity, int selection) {
	if (m_iViewers.none())
		return;
	if (IsPlayerViewing(pEntity)) {
		int index = selection - 1;
		if (index < 0)
			index = m_aryOption.size() - 1;
		auto item = m_aryOption[index];
		if (item != nullptr)
			item->Excute(pEntity, selection);
	}
}

void CEccoTextMenuExecutor::AddItem(CBaseEccoExecutor* pItem) {
	if(m_aryOption.size() >= MAX_MENU_OPTIONS) {
		LOG_MESSAGE(PLID, "Cannot add more than %d items to a text menu", MAX_MENU_OPTIONS);
		return;
	}
	m_aryOption.push_back(pItem);
}

void CEccoTextMenuExecutor::Excute(edict_t* pPlayer, int selection) {
	//Build message
	std::string buffer = GetDisplayName(pPlayer) + "\n\n";
	std::bitset<16> validslots;
	for (size_t i = 0; i < m_aryOption.size(); i++) {
		auto item = m_aryOption[i];
		bool valid = item != nullptr;
		validslots.set(i, valid);
		if (!valid) {
			buffer += std::to_string(i == m_aryOption.size() - 1 ? 0 : i + 1) + ". ";
			buffer += item->GetDisplayName(pPlayer);
		}
		buffer += '\n';
	}
	bool validplayer = IsValidPlayer(pPlayer);
	if (validplayer)
		MESSAGE_BEGIN(MSG_ONE, MSG_ShowMenu, NULL, pPlayer);
	else
		MESSAGE_BEGIN(MSG_ALL, MSG_ShowMenu);
	WRITE_SHORT(validslots.to_ulong());
	WRITE_CHAR(m_iDuration);
	WRITE_BYTE(FALSE); // "need more" (?)
	WRITE_STRING(buffer.c_str());
	MESSAGE_END();
	if (validplayer)
		SetPlayerViewing(pPlayer, true);
	else{
		m_iViewers.reset().flip();
		for (size_t i = 0; i < g_aryTextMenus.size(); i++) {
			g_aryTextMenus[i] = this;
		}
	}
}
// Shit, wootguy didnt finish TextMenu
// Have to write by myself :(
#include <bitset>

#include "meta_utility.h"
#include "lang/lang.h"
#include "CEccoTextMenuExecutor.h"
#include "config/CConfig.h"

std::array<CEccoTextMenuExecutor*, MAX_PLAYERS> g_aryTextMenus;

extern int g_msgShowMenu;
using namespace EccoMetaUtility;

// listen for any other functions/plugins opening menus, so that TextMenu knows if it's the active menu
void TextMenuMessageBeginHook(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed) {
	if (msg_type != g_msgShowMenu)
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
	if (!StringCaseCmp(CMD_ARGV(0), "menuselect")) {
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
	return m_aryViewers[index].IsViewing();
}

void CEccoTextMenuExecutor::SetPlayerViewing(edict_t* ent, bool view){
	int index = ENTINDEX(ent) - 1;
	m_aryViewers[index].Viewing = view;
	float keep_time = GetEccoConfig()->BuyMenu.KeepOpenTime;
	float game_time = g_engfuncs.pfnTime();
	m_aryViewers[index].StopVIewTime = view ? (game_time + keep_time) : 0.0f;
	g_aryTextMenus[index] = view ? this : nullptr;
}

void CEccoTextMenuExecutor::HandleMenuMessage(int msg_dest, edict_t* ed) {
	// Another text menu has been opened for one or more players, so this menu
	// is no longer visible and should not handle menuselect commands

	// If this message is in fact triggered by this object, then the viewer flags should be set
	// after this func finishes
	if (NoneViewer())
		return;

	if ((msg_dest == MSG_ONE || msg_dest == MSG_ONE_UNRELIABLE) && ed)
		SetPlayerViewing(ed, false);
	else if (msg_dest == MSG_ALL) {
		for (auto& view : m_aryViewers) {
			view.SetViewing(false);
		}
		for (size_t i = 0; i < g_aryTextMenus.size(); i++) {
			g_aryTextMenus[i] = nullptr;
		}
	}
	else
		LOG_MESSAGE(PLID, "Who the hell use msg_dest: %d in ShowMenu message????", msg_dest);
}

void CEccoTextMenuExecutor::HandleMenuselectCmd(edict_t* pEntity, int selection) {
	if (NoneViewer())
		return;
	if (IsPlayerViewing(pEntity)) {
		auto item = m_aryOption[selection];
		if (item != nullptr)
			item->Excute(pEntity, selection);
	}
}

size_t CEccoTextMenuExecutor::GetSize() const{
	return m_iSize;
}

CBaseEccoExecutor* CEccoTextMenuExecutor::GetOption(size_t index){
	return m_aryOption[index];
}

bool CEccoTextMenuExecutor::NoneViewer(){
	bool none = true;
	for (auto& prop : m_aryViewers) {
		none &= !prop.Viewing;
	}
	return none;
}

void CEccoTextMenuExecutor::FlipVIewer(){

}

void CEccoTextMenuExecutor::AddItem(CBaseEccoExecutor* pItem) {
	if(m_iSize >= MAX_MENU_OPTIONS) {
		LOG_MESSAGE(PLID, "Cannot add more than %d items to a text menu", MAX_MENU_OPTIONS);
		return;
	}
	m_aryOption[m_iSize] = pItem;
	m_iSize++;
}

std::string CEccoTextMenuExecutor::GetDisplayTitle(edict_t* pPlayer) const {
	if(m_szTitle.empty())
		return GetTranslation(pPlayer, m_szId);
	return GetTranslation(pPlayer, m_szTitle);
}

void CEccoTextMenuExecutor::Close(edict_t* pent) {
	if (pent)
		SetPlayerViewing(pent, false);
	else {
		for (size_t i = 0; i < m_aryViewers.size(); i++) {
			if(m_aryViewers[i].Viewing)
				g_aryTextMenus[i] = nullptr;
		}
		for (auto& view : m_aryViewers) {
			view.SetViewing(false);
		}
	}
}

void CEccoTextMenuExecutor::Excute(edict_t* pPlayer, int selection) {
	//Build message
	std::string buffer = GetDisplayTitle(pPlayer) + "\n\n";
	std::bitset<16> validslots;
	for (size_t i = 0; i < MAX_MENU_OPTIONS; i++) {
		auto item = m_aryOption[i];
		bool valid = item != nullptr;
		validslots.set(i, valid);
		if (valid) {
			buffer += std::to_string(i == MAX_MENU_OPTIONS - 1 ? 0 : i + 1) + ". ";
			buffer += item->GetDisplayName(pPlayer);
		}
		buffer += '\n';
	}
	bool validplayer = IsValidPlayer(pPlayer);
	if (validplayer)
		MESSAGE_BEGIN(MSG_ONE, g_msgShowMenu, NULL, pPlayer);
	else
		MESSAGE_BEGIN(MSG_ALL, g_msgShowMenu);

	byte duration = static_cast<byte>(GetEccoConfig()->BuyMenu.KeepOpenTime * 10);
	WRITE_SHORT(validslots.to_ulong());
	WRITE_CHAR(duration);
	WRITE_BYTE(FALSE); // "need more" (?)
	WRITE_STRING(buffer.c_str());
	MESSAGE_END();
	if (validplayer)
		SetPlayerViewing(pPlayer, true);
	else{
		for (auto& view : m_aryViewers) {
			view.SetViewing(true);
		}
		for (size_t i = 0; i < g_aryTextMenus.size(); i++) {
			g_aryTextMenus[i] = this;
		}
	}
	CBaseEccoExecutor::Excute(pPlayer, selection);
}

void CEccoTextMenuExecutor::viewer_prop_s::SetViewing(bool view){
	Viewing = view;
	StopVIewTime = view ? (g_engfuncs.pfnTime() + GetEccoConfig()->BuyMenu.KeepOpenTime) : 0.0f;
}

bool CEccoTextMenuExecutor::viewer_prop_s::IsViewing() const{
	float game_time = g_engfuncs.pfnTime();
	return Viewing && (game_time < StopVIewTime);
}

void CEccoTextMenuExecutor::viewer_prop_s::Flip(){
	Viewing = !Viewing;
	SetViewing(Viewing);
}

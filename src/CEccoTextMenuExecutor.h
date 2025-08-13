// Shit, wootguy didnt finish TextMenu
// Have to write by myself :(

#pragma once
#include <array>
#include <bitset>

#include <extdll.h>
#include <meta_api.h>

#include "CBaseEccoExecutor.h"

constexpr int MAX_MENU_OPTIONS = 10;
constexpr int EXITOPTION_INDEX = 10;
constexpr int MAX_PLAYERS = 32;
constexpr int ITEMS_PER_PAGE = 7;

// this must be called as part of a MessageBegin hook for text menus to know when they are active
extern void TextMenuMessageBeginHook(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed);

// this must be called as part of a DLL ClientCommand hook for option selections to work
extern bool TextMenuClientCommandHook(edict_t* pEntity);

class CEccoTextMenuExecutor : public CBaseEccoExecutor {
public:
	void AddItem(CBaseEccoExecutor* pItem);

	// set player to NULL to send to all players.
	virtual void Excute(edict_t* pPlayer, int selection) override;

	std::string GetDisplayTitle(edict_t* pPlayer);

	bool IsPlayerViewing(edict_t* ent);
	void SetPlayerViewing(edict_t* ent, bool view);

	// don't call directly. This is triggered by global hook functions
	void HandleMenuMessage(int msg_dest, edict_t* ed);
	// don't call directly. This is triggered by global hook functions
	void HandleMenuselectCmd(edict_t* pEntity, int selection);

	CBaseEccoExecutor* m_pParent = nullptr; // parent menu, if any

	std::string m_szTitle;
private:
	int m_iDuration = 255; // how long the menu shuold be displayed for
	std::bitset<MAX_PLAYERS> m_iViewers = 0; // bitfield indicating who can see the menu
	std::array<CBaseEccoExecutor*, MAX_MENU_OPTIONS> m_aryOption; //ONLY 10 options
	size_t m_iSize = 0; // current size of the menu, used to determine if the menu is full
};


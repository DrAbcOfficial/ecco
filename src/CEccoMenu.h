// Most are copied from wootguy's mmlib

#pragma once
#include <vector>
#include <meta_api.h>

#include "IEccoExcuter.h"
#include "CEccoMenuItem.hpp"

constexpr int MAX_MENU_OPTIONS = 128;
constexpr int EXITOPTION_INDEX = 10;
constexpr int MAX_PLAYERS = 32;
constexpr int ITEMS_PER_PAGE = 7;

// this must be called as part of a MessageBegin hook for text menus to know when they are active
void TextMenuMessageBeginHook(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed);

// this must be called as part of a DLL ClientCommand hook for option selections to work
bool TextMenuClientCommandHook(edict_t* pEntity);

class CEccoMenu : public IEccoExcuter {
public:
	void SetTitle(const char* title);
	void AddItem(CEccoMenuItem* pItem);

	// set player to NULL to send to all players.
	// This should be the same target as was used with initMenuForPlayer
	// paging not supported yet
	virtual void Excute(edict_t* pPlayer, int8_t iPage, bool bReOpen) override;

	// don't call directly. This is triggered by global hook functions
	void HandleMenuMessage(int msg_dest, edict_t* ed);

	// don't call directly. This is triggered by global hook functions
	void HandleMenuselectCmd(edict_t* pEntity, int selection);

private:
	inline int GetOptionSize();
	int m_iDuration = 255; // how long the menu shuold be displayed for
	float m_fOpenTime = 0; // time when the menu was opened
	uint32_t m_iViewers = 0; // bitfield indicating who can see the menu
	std::string m_szTitle;
	std::vector<CEccoMenuItem*> m_aryOption;
	int8_t m_iLastPage = 0;
	int8_t m_iLastDuration = 0; // always 0

	bool m_bIsActive = false;

	bool IsPaginated();
};


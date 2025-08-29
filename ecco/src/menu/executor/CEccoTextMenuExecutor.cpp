#include <bitset>

#include "lang/lang.h"
#include "CEccoTextMenuExecutor.h"
#include "config/CConfig.h"

#include "meta_utility.h"

using namespace EccoMetaUtility;

std::array<CEccoTextMenuExecutor*, MAX_PLAYERS> g_aryTextMenus{};
extern int g_msgShowMenu;

void TextMenuMessageBeginHook(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed) {
    if (msg_type != g_msgShowMenu) 
        return;
    if ((msg_dest == MSG_ONE || msg_dest == MSG_ONE_UNRELIABLE) && IsValidPlayer(ed)) {
        const int index = ENTINDEX(ed) - 1;
        if (g_aryTextMenus[index])
            g_aryTextMenus[index]->HandleMenuMessage(msg_dest, ed);
    }
    else if (msg_dest == MSG_ALL) {
        for (auto menu : g_aryTextMenus) {
            if (menu) 
                menu->HandleMenuMessage(msg_dest, ed);
        }
    }
}

bool TextMenuClientCommandHook(edict_t* pEntity) {
    if (StringCaseCmp(CMD_ARGV(0), "menuselect") != 0) 
        return false;

    const int selection = atoi(CMD_ARGV(1)) - 1;
    if (selection < 0 || selection >= MAX_MENU_OPTIONS) 
        return true;
    const int index = ENTINDEX(pEntity) - 1;
    if (g_aryTextMenus[index]) {
        g_aryTextMenus[index]->HandleMenuselectCmd(pEntity, selection);
    }
    return true;
}

void CEccoTextMenuExecutor::viewer_prop_s::SetViewing(bool view) {
    viewing = view;
    stopViewTime = view ? (g_engfuncs.pfnTime() + GetEccoConfig()->BuyMenu.KeepOpenTime) : 0.0f;
}
bool CEccoTextMenuExecutor::viewer_prop_s::IsViewing() const {
    return viewing && (g_engfuncs.pfnTime() < stopViewTime);
}
bool CEccoTextMenuExecutor::IsPlayerViewing(edict_t* ent) {
    return IsValidPlayer(ent) && m_aryViewers[ENTINDEX(ent) - 1].IsViewing();
}
void CEccoTextMenuExecutor::SetPlayerViewing(edict_t* ent, bool view) {
    if (!IsValidPlayer(ent)) 
        return;

    const int index = ENTINDEX(ent) - 1;
    m_aryViewers[index].SetViewing(view);
    g_aryTextMenus[index] = view ? this : nullptr;
}
void CEccoTextMenuExecutor::HandleMenuMessage(int msg_dest, edict_t* ed) {
    if (NoneViewer()) 
        return;

    if ((msg_dest == MSG_ONE || msg_dest == MSG_ONE_UNRELIABLE) && ed) {
        SetPlayerViewing(ed, false);
    }
    else if (msg_dest == MSG_ALL) {
        for (auto& viewer : m_aryViewers) {
            viewer.SetViewing(false);
        }
        std::fill(g_aryTextMenus.begin(), g_aryTextMenus.end(), nullptr);
    }
}
void CEccoTextMenuExecutor::HandleMenuselectCmd(edict_t* pEntity, int selection) {
    if (NoneViewer() || !IsPlayerViewing(pEntity)) 
        return;
    auto item = m_aryOption[selection];
    if (item) 
        item->Excute(pEntity, selection);
}

bool CEccoTextMenuExecutor::NoneViewer() const {
    for (const auto& viewer : m_aryViewers) {
        if (viewer.IsViewing()) 
            return false;
    }
    return true;
}

void CEccoTextMenuExecutor::AddItem(CBaseEccoExecutor* pItem) {
    if (m_iSize >= MAX_MENU_OPTIONS) {
        LOG_MESSAGE(PLID, "Max %d per menu page!", MAX_MENU_OPTIONS);
        return;
    }
    m_aryOption[m_iSize++] = pItem;
}

std::string CEccoTextMenuExecutor::GetDisplayTitle(edict_t* pPlayer) const {
    return m_szTitle.empty() ? GetTranslation(pPlayer, m_szId) : GetTranslation(pPlayer, m_szTitle);
}

void CEccoTextMenuExecutor::Close(edict_t* pent) {
    if (pent) {
        SetPlayerViewing(pent, false);
    }
    else {
        for (size_t i = 0; i < m_aryViewers.size(); ++i) {
            if (m_aryViewers[i].viewing) g_aryTextMenus[i] = nullptr;
        }
        for (auto& viewer : m_aryViewers) {
            viewer.SetViewing(false);
        }
    }
}

void CEccoTextMenuExecutor::Excute(edict_t* pPlayer, int selection) {
    std::string buffer = GetDisplayTitle(pPlayer) + "\n\n";
    std::bitset<16> validSlots;
    for (size_t i = 0; i < MAX_MENU_OPTIONS; ++i) {
        auto item = m_aryOption[i];
        const bool valid = (item != nullptr);
        validSlots.set(i, valid);
        if (valid) {
            buffer += std::to_string((i == MAX_MENU_OPTIONS - 1) ? 0 : i + 1) + ". "
                + item->GetDisplayName(pPlayer);
        }
        buffer += '\n';
    }

    const bool isSinglePlayer = IsValidPlayer(pPlayer);
    isSinglePlayer ? MESSAGE_BEGIN(MSG_ONE, g_msgShowMenu, nullptr, pPlayer)
        : MESSAGE_BEGIN(MSG_ALL, g_msgShowMenu);

    WRITE_SHORT(validSlots.to_ulong());
    WRITE_CHAR(static_cast<char>(GetEccoConfig()->BuyMenu.KeepOpenTime));
    WRITE_BYTE(FALSE); // "need more"
    WRITE_STRING(buffer.c_str());
    MESSAGE_END();

    if (isSinglePlayer) {
        SetPlayerViewing(pPlayer, true);
    }
    else {
        for (auto& viewer : m_aryViewers) {
            viewer.SetViewing(true);
        }
        std::fill(g_aryTextMenus.begin(), g_aryTextMenus.end(), this);
    }
    CBaseEccoExecutor::Excute(pPlayer, selection);
}
#pragma once
#include <array>
#include <string>

#include <extdll.h>
#include <meta_api.h>

#include "menu/executor/CBaseEccoExecutor.h"

constexpr int MAX_MENU_OPTIONS = 10;
constexpr int MAX_PLAYERS = 32;
constexpr int MAX_ITEMS_PER_PAGE = 7;
constexpr int MAX_ITEM_NO_PAGE = 9;

extern void TextMenuMessageBeginHook(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed);
extern bool TextMenuClientCommandHook(edict_t* pEntity);

class CEccoTextMenuExecutor : public CBaseEccoExecutor {
public:
    void AddItem(CBaseEccoExecutor* pItem);
    void Excute(edict_t* pPlayer, int selection) override;
    void Close(edict_t* pent);
    std::string GetDisplayTitle(edict_t* pPlayer) const;
    bool IsPlayerViewing(edict_t* ent);
    void SetPlayerViewing(edict_t* ent, bool view);
    size_t GetSize() const { return m_iSize; }
    CBaseEccoExecutor* GetOption(size_t index) { return m_aryOption[index]; }

    std::string m_szTitle;
    CBaseEccoExecutor* m_pParent = nullptr;

private:
    friend void TextMenuMessageBeginHook(int, int, const float*, edict_t*);
    friend bool TextMenuClientCommandHook(edict_t*);

    struct viewer_prop_s {
        bool viewing = false;
        float stopViewTime = 0.0f;

        void SetViewing(bool view);
        bool IsViewing() const;
        void Flip() { viewing = !viewing; SetViewing(viewing); }
    };

    void HandleMenuMessage(int msg_dest, edict_t* ed);
    void HandleMenuselectCmd(edict_t* pEntity, int selection);


    bool NoneViewer() const;

    std::array<viewer_prop_s, MAX_PLAYERS> m_aryViewers{};
    std::array<CBaseEccoExecutor*, MAX_MENU_OPTIONS> m_aryOption{};
    size_t m_iSize = 0;
};
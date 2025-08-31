#include <charconv>
#include <ranges>

#include "config/CConfig.h"

#include "meta_utility.h"

extern int g_msgTextMsg;

namespace EccoMetaUtility{
    void WriteRGBA(unsigned int rgba)
    {
        WRITE_BYTE(static_cast<unsigned char>((rgba >> 24) & 0xFF));  // R
        WRITE_BYTE(static_cast<unsigned char>((rgba >> 16) & 0xFF));  // G
        WRITE_BYTE(static_cast<unsigned char>((rgba >> 8) & 0xFF));   // B
        WRITE_BYTE(static_cast<unsigned char>(rgba & 0xFF));          // A
    }

    std::string GetPlayerSteamId(edict_t* pent){
        if (FNullEnt(pent))
            return "";
        const char* infobuffer = GET_INFOKEYBUFFER(pent);
        return infobuffer ? INFOKEY_VALUE(const_cast<char*>(infobuffer), const_cast<char*>("*sid")) : "";
    }

    edict_t* GetPlayerBySteamId(const char* steamid){
        if (!steamid || !*steamid)
            return nullptr;
        for (int i = 1; i < 33; i++){
            edict_t* pent = INDEXENT(i);
            if (IsValidPlayer(pent) && StringCaseCmp(GetPlayerSteamId(pent).c_str(), steamid) == 0)
                return pent;
        }
        return nullptr;
    }

    std::string_view GetGameDir(){
        static const std::string_view dir = GET_GAME_INFO(PLID, GINFO_GAMEDIR);
        return dir;
    }

    void FakeClientCommand(edict_t* pent, const char* cmd){
        if (!pent || !cmd)
            return;
        MESSAGE_BEGIN(MSG_ONE, 9, nullptr, pent);
        WRITE_STRING(cmd);
        MESSAGE_END();
    }

    inline void DoClientPrintf(edict_t* target, int msgType, ClientPrintTarget hud, const char* text){
        if (!text)
            return;
        std::string temp = text;
        temp += '\n';
        MESSAGE_BEGIN(msgType, g_msgTextMsg, nullptr, target);
        WRITE_BYTE(static_cast<int>(hud));
        WRITE_STRING(temp.c_str());
        MESSAGE_END();
    }

    void ClientPrintf(edict_t* target, ClientPrintTarget hud, const char* text){
        if (!target)
            return;
        DoClientPrintf(target, MSG_ONE, hud, text);
    }

    void ClientPrintfAll(ClientPrintTarget hud, const char* text){
        DoClientPrintf(nullptr, MSG_BROADCAST, hud, text);
    }

    int StringToInteger(const std::string& s){
        if (s.empty())
            return 0;
        size_t start = s.find_first_not_of(" \t\n\r");
        if (start == std::string::npos)
            return 0;
        int value = 0;
        const char* first = s.data() + start;
        const char* last = s.data() + s.size();
        auto [ptr, ec] = std::from_chars(first, last, value);
        if (ec == std::errc() && ptr == last)
            return value;
        else
            return 0;
    }

    std::string TrimString(const std::string& s){
        auto is_space = [](char c){
                return std::isspace(static_cast<unsigned char>(c));
        };
        auto trimmed = s | std::views::drop_while(is_space)
            | std::views::reverse
            | std::views::drop_while(is_space)
            | std::views::reverse;
        return std::string(trimmed.begin(), trimmed.end());
    }

    edict_t* PrivateToEdict(void* PrivateData){
        if (!PrivateData)
            return nullptr;
        constexpr size_t OFFSET_ENTVAR = 4;
        entvars_t** pev_ptr = reinterpret_cast<entvars_t**>(static_cast<char*>(PrivateData) + OFFSET_ENTVAR);
        if (pev_ptr && *pev_ptr)
            return (*pev_ptr)->pContainingEntity;
        else
            return nullptr;
    }

    void PrecacheOther(const char* szClassname) {
        edict_t* pent;
        pent = CREATE_NAMED_ENTITY(MAKE_STRING(szClassname));
        if (FNullEnt(pent)) {
            ALERT(at_console, "NULL Ent in PrecacheOther\n");
            return;
        }
        void* pEntity = pent->pvPrivateData;
        if (!pEntity) {
            ALERT(at_console, "NULL PrivateData in PrecacheOther\n");
            return;
        }

#ifdef _WIN32
#define SERVER_DECL __fastcall
#define SERVER_DUMMYARG , int dummy
#define SERVER_PASS_DUMMYARG , 0
#else
#define SERVER_DECL 
#define SERVER_DUMMYARG
#define SERVER_PASS_DUMMYARG
#endif
        using PrecacheFunc = void (SERVER_DECL*)(void* pThis SERVER_DUMMYARG);
        char** vtable = *reinterpret_cast<char***>(pEntity);
        PrecacheFunc pfnPrecache = reinterpret_cast<PrecacheFunc>(vtable[(GetEccoConfig()->PrecacheOffset * 8)]);
        if (pfnPrecache)
            pfnPrecache(pEntity SERVER_PASS_DUMMYARG);
        REMOVE_ENTITY(pent);
#undef SERVER_DECL
#undef SERVER_DUMMYARG
#undef SERVER_PASS_DUMMYARG
    }
}
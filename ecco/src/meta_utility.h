#pragma once
#include <extdll.h>
#include <string>
#include <string_view>
#include <meta_api.h>

#ifdef WIN32
#define StringNCaseCmp _strnicmp
#define StringCaseCmp _stricmp
#else
#define StringNCaseCmp strncasecmp
#define StringCaseCmp strcasecmp
#endif

namespace EccoMetaUtility
{
    inline void WriteFloat(float value)
    {
        WRITE_LONG(*reinterpret_cast<int*>(&value));
    }

    void WriteRGBA(unsigned int rgba);

    std::string GetPlayerSteamId(edict_t* pent);
    edict_t* GetPlayerBySteamId(const char* steamid);

    inline bool IsValidPlayer(edict_t* pentPlayer)
    {
        return pentPlayer
            && (pentPlayer->v.flags & FL_CLIENT)
            && !(pentPlayer->v.flags & FL_PROXY)
            && STRING(pentPlayer->v.netname)[0] != '\0';
    }

    std::string_view GetGameDir();

    void FakeClientCommand(edict_t* pent, const char* cmd);

    enum class ClientPrintTarget {
        Notify = 1,
        Console = 2,
        Talk = 3,
        Center = 4
    };

    void ClientPrintf(edict_t* target, ClientPrintTarget hud, const char* text);
    void ClientPrintfAll(ClientPrintTarget hud, const char* text);

    int StringToInteger(const std::string& s);
    std::string TrimString(const std::string& s);
    edict_t* PrivateToEdict(void* PrivateData);

    void PrecacheOther(const char* szClassname);
}

#define WRITE_FLOAT EccoMetaUtility::WriteFloat
#define WRITE_RGBA EccoMetaUtility::WriteRGBA
#pragma once
#include <extdll.h>
#include <string>

#ifdef WIN32
#define StringNCaseCmp(a,b,n) _strnicmp(a,b,n)
#define StringCaseCmp(a,b) _stricmp(a,b)
#else
#define StringNCaseCmp(a,b,n) strncasecmp(a,b,n)
#define StringCaseCmp(a,b) strcasecmp(a,b)
#endif

namespace EccoMetaUtility
{
	extern void WriteFloat(float value);
	extern void WriteRGBA(unsigned int rgba);

	extern std::string GetPlayerSteamId(edict_t* pent);
	extern edict_t* GetPlayerBySteamId(const char* steamid);
	extern bool IsValidPlayer(edict_t* pentPlayer);
	extern std::string_view& GetGameDir();
	extern void FakeClientCommand(edict_t* pent, const char* cmd);

	enum class ClientPrintTarget {
		Notify = 1,
		Console = 2,
		Talk = 3,
		Center = 4
	};
	extern void ClientPrintf(edict_t* target, ClientPrintTarget hud, const char* text);
	extern void ClientPrintfAll(ClientPrintTarget hud, const char* text);
	extern int StringToInterger(const std::string& s);
	extern std::string TrimString(const std::string& s);
	extern edict_t* PrivateToEdict(void* PrivateData);
}

#define WRITE_FLOAT EccoMetaUtility::WriteFloat
#define WRITE_RGBA EccoMetaUtility::WriteRGBA
//Copied from mmlib! some functions...

#include <charconv>
#include <ranges>

#include "meta_utility.h"
#include <meta_api.h>

extern int g_msgTextMsg;

inline void EccoMetaUtility::WriteFloat(float value){
	WRITE_LONG(*reinterpret_cast<int*>(&value));
}

void EccoMetaUtility::WriteRGBA(unsigned int rgba){
	unsigned char r = (rgba & 0xFF000000) >> 24;
	unsigned char g = (rgba & 0xFF0000) >> 16;
	unsigned char b = (rgba & 0xFF00) >> 8;
	unsigned char a = rgba & 0xFF;
	WRITE_BYTE(r);
	WRITE_BYTE(g);
	WRITE_BYTE(b);
	WRITE_BYTE(a);
}

std::string EccoMetaUtility::GetPlayerSteamId(edict_t* pent){
	const char* infobuffer = GET_INFOKEYBUFFER(pent);
	const char* id = INFOKEY_VALUE(const_cast<char*>(infobuffer), const_cast<char*>("*sid"));
	return id;
}

edict_t* EccoMetaUtility::GetPlayerBySteamId(const char* steamid) {
	for (int i = 1; i < 33; i++) {
		edict_t* pent = INDEXENT(i);
		if (IsValidPlayer(pent)) {
			const char* infobuffer = GET_INFOKEYBUFFER(pent);
			const char* id = INFOKEY_VALUE(const_cast<char*>(infobuffer), const_cast<char*>("*sid"));
			if (!strcmp(id, steamid))
				return pent;
		}
	}
	return nullptr;
}

bool EccoMetaUtility::IsValidPlayer(edict_t* pentPlayer) {
	return pentPlayer && (pentPlayer->v.flags & FL_CLIENT) != 0 && (pentPlayer->v.flags & FL_PROXY) == 0 && STRING(pentPlayer->v.netname)[0] != '\0';
}

std::string_view& EccoMetaUtility::GetGameDir(){
	static std::string_view dir = GET_GAME_INFO(PLID, GINFO_GAMEDIR);
	return dir;
}

void EccoMetaUtility::FakeClientCommand(edict_t* pent, const char* cmd){
	//SVC_STUFFTEXT
	MESSAGE_BEGIN(MSG_ONE, 9, nullptr, pent);
		WRITE_STRING(cmd);
	MESSAGE_END();
}

void EccoMetaUtility::ClientPrintf(edict_t* target, ClientPrintTarget hud, const char* text){
	std::string temp = text;
	temp += '\n';
	MESSAGE_BEGIN(MSG_ONE, g_msgTextMsg, nullptr, target);
		WRITE_BYTE((int)hud);
		WRITE_STRING(temp.c_str());
	MESSAGE_END();
}

void EccoMetaUtility::ClientPrintfAll(ClientPrintTarget hud, const char* text){
	std::string temp = text;
	temp += '\n';
	MESSAGE_BEGIN(MSG_BROADCAST, g_msgTextMsg);
		WRITE_BYTE((int)hud);
		WRITE_STRING(temp.c_str());
	MESSAGE_END();
}

int EccoMetaUtility::StringToInterger(const std::string& s) {
	if (s.empty())
		return 0;
	size_t start = 0;
	while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
		++start;
	}
	if (start == s.size())
		return 0;
	int value = 0;
	const char* first = s.data() + start;
	const char* last = s.data() + s.size();
	auto [ptr, ec] = std::from_chars(first, last, value);
	if (ec == std::errc() && ptr == last)
		return value;
	else
		return 0;
	};
std::string EccoMetaUtility::TrimString(const std::string& s) {
	auto is_space = [](char c) {
		return std::isspace(static_cast<unsigned char>(c)) != 0;
	};
	auto trimmed_view = s | std::views::drop_while(is_space)
		| std::views::reverse
		| std::views::drop_while(is_space)
		| std::views::reverse;
	return std::string(trimmed_view.begin(), trimmed_view.end());
}

#pragma once
#include <string>
#include <vector>

#include <extdll.h>
#include <meta_api.h>
#include "meta_utility.h"

extern std::string& GetTranslation(edict_t* player, std::string& key);
extern void LoadTranslations();

std::u8string StringToU8String(std::string str);
std::string U8StringToString(std::u8string str);
std::u8string LangReplacePlaceholders(std::u8string text, const std::vector<std::u8string>& args);
void ClientPrintfTranslation(edict_t* target, ClientPrintTarget hud, std::string text_key);
template <typename... Strings>
void ClientPrintfTranslation(edict_t* target, ClientPrintTarget hud, std::string text_key, Strings&&... args) {
    std::u8string u8lang_text = StringToU8String(GetTranslation(target, text_key));
    std::vector<std::u8string> args_list = { std::forward<Strings>(args)... };
	std::string final_text = U8StringToString(LangReplacePlaceholders(u8lang_text, args_list));
    ClientPrintf(target, hud, final_text.c_str());
}
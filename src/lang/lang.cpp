#include <format>
#include <filesystem>
#include <map>
#include <tomlplusplus/toml.hpp>

#include "storage/Storage.h"

#include "config/CConfig.h"

#include "lang.h"

#include <meta_api.h>

using lang_pair_t = struct lang_pair_s{
    std::string key;
    std::string translation;
};

static std::map<std::string, std::vector<lang_pair_t*>> s_mapTranslations;

std::string& GetTranslation(edict_t* player, std::string& key){
	std::string player_lang = GetPlayerStorageItem(player)->GetLang();
	auto it = s_mapTranslations.find(player_lang);
    if (it == s_mapTranslations.end())
        it = s_mapTranslations.find(GetEccoConfig()->DefaultLang);
    if (it == s_mapTranslations.end())
        it = s_mapTranslations.begin();
    auto trans_it = std::find_if(it->second.begin(), it->second.end(),
        [&key](const lang_pair_t* pair) { return pair->key == key; });
    if (trans_it != it->second.end())
        return (*trans_it)->translation;
    static std::string temp = key;
    return temp;
}

void LoadTranslations(){
    std::filesystem::path p(GetGameDir());
	p.append("addons/ecco/lang");
    if (!std::filesystem::exists(p)) {
        LOG_CONSOLE(PLID, "Ecco language directory does not exist: %s", p.string().c_str());
        return;
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
        if (entry.is_regular_file() && entry.path().extension() == ".toml") {
            try {
				std::string lang_name = entry.path().stem().string();
                auto config = toml::parse_file(entry.path().string());
                auto terverse = config["Lang"].as_table();
                for (auto iter = terverse->begin(); iter != terverse->end(); iter++) {
                    lang_pair_t* lang_pair = new lang_pair_t();
					lang_pair->key = iter->first;
                    lang_pair->translation = iter->second.value_or("");
					s_mapTranslations[lang_name].push_back(lang_pair);
                }
            } 
            catch (const toml::parse_error& err) {
                LOG_ERROR(PLID, "Error parsing translation file %s: %s", entry.path().string().c_str(), err.description());
            }
        }
	}
}

std::u8string StringToU8String(std::string str){
    return std::u8string(str.begin(), str.end());
}

std::string U8StringToString(std::u8string str){
    return std::string(str.begin(), str.end());
}

std::u8string LangReplacePlaceholders(std::u8string text, const std::vector<std::u8string>& args){
    for (size_t i = 0; i < args.size() && i <= 5; ++i) {
		std::string tempholder = std::format("{{{}}}", i);
        std::u8string placeholder(tempholder.begin(), tempholder.end());
        size_t pos = 0;
        while ((pos = text.find(placeholder, pos)) != std::string::npos) {
            text.replace(pos, placeholder.length(), args[i]);
            pos += args[i].length();
        }
    }
    return text;
}

void ClientPrintfTranslation(edict_t* target, ClientPrintTarget hud, std::string text_key) {
    ClientPrintf(target, hud, GetTranslation(target, text_key).c_str());
}
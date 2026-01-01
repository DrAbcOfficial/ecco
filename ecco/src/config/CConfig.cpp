#include <tomlplusplus/toml.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <meta_utility.h>
#include "config/CConfig.h"

#undef close

static CEccoConfig s_pEccoConfig;
static std::vector<std::string> s_aryBannedMaps{};
static std::map<std::string, ConfigValue> s_configMap;


static void RegisterConfigUInt(const std::string& path, uint32_t& value) {
	s_configMap[path] = ConfigValue{
		ConfigValueType::UInt,
		[&value]() -> std::string { return std::to_string(value); },
		[&value](const std::string& str) -> bool {
			try {
				value = std::stoi(str);
				return true;
			}
 catch (...) {
  return false;
}
}
	};
}

static void RegisterConfigInt(const std::string& path, int& value) {
	s_configMap[path] = ConfigValue{
		ConfigValueType::Int,
		[&value]() -> std::string { return std::to_string(value); },
		[&value](const std::string& str) -> bool {
			try {
				value = std::stoi(str);
				return true;
			} catch (...) {
				return false;
			}
		}
	};
}

static void RegisterConfigFloat(const std::string& path, float& value) {
	s_configMap[path] = ConfigValue{
		ConfigValueType::Float,
		[&value]() -> std::string { return std::to_string(value); },
		[&value](const std::string& str) -> bool {
			try {
				value = std::stof(str);
				return true;
			} catch (...) {
				return false;
			}
		}
	};
}

static void RegisterConfigString(const std::string& path, std::string& value) {
	s_configMap[path] = ConfigValue{
		ConfigValueType::String,
		[&value]() -> std::string { return value; },
		[&value](const std::string& str) -> bool {
			value = str;
			return true;
		}
	};
}

static void RegisterConfigBool(const std::string& path, bool& value) {
	s_configMap[path] = ConfigValue{
		ConfigValueType::Bool,
		[&value]() -> std::string { return value ? "true" : "false"; },
		[&value](const std::string& str) -> bool {
			if (str == "true" || str == "1") {
				value = true;
				return true;
			} else if (str == "false" || str == "0") {
				value = false;
				return true;
			}
			return false;
		}
	};
}

static void RegisterConfigVectorString(const std::string& path, std::vector<std::string>& value) {
	s_configMap[path] = ConfigValue{
		ConfigValueType::VectorString,
		[&value]() -> std::string {
			std::string result = "[";
			for (size_t i = 0; i < value.size(); ++i) {
				if (i > 0) result += ", ";
				result += "\"" + value[i] + "\"";
			}
			result += "]";
			return result;
		},
		[&value](const std::string& str) -> bool {
			try {
				value.clear();
				size_t start = str.find('[');
				size_t end = str.rfind(']');
				if (start == std::string::npos || end == std::string::npos) {
					return false;
				}
				
				std::string content = str.substr(start + 1, end - start - 1);
				std::istringstream iss(content);
				std::string token;
				
				while (std::getline(iss, token, ',')) {
					size_t quote_start = token.find('"');
					size_t quote_end = token.rfind('"');
					if (quote_start != std::string::npos && quote_end != std::string::npos && quote_start < quote_end) {
						value.push_back(token.substr(quote_start + 1, quote_end - quote_start - 1));
					}
				}
				return true;
			} catch (...) {
				return false;
			}
		}
	};
}

CEccoConfig* GetEccoConfig(){
	return &s_pEccoConfig;
}

bool IsBannedMap(const char* map){
	return std::find(s_aryBannedMaps.begin(), s_aryBannedMaps.end(), map) != s_aryBannedMaps.end();
}

bool LoadEccoConfig(){
	std::filesystem::path gamedir(EccoMetaUtility::GetGameDir());
	gamedir.append("addons/ecco/Config.toml");
	s_configMap.clear();
	try{
		auto toml = toml::parse_file(gamedir.string());
#define GET_VALUE_RAW(section, element, def) \
	s_pEccoConfig.section.element = toml["Ecco"][#section][#element].value_or(def);
#define GET_VALUE_OR_INT(section, element, def) \
	GET_VALUE_RAW(section, element, def) \
	RegisterConfigInt(#section "." #element, s_pEccoConfig.section.element)
#define GET_VALUE_OR_UINT(section, element, def) \
	GET_VALUE_RAW(section, element, def) \
	RegisterConfigUInt(#section "." #element, s_pEccoConfig.section.element)
#define GET_VALUE_OR_FLOAT(section, element, def) \
	GET_VALUE_RAW(section, element, def) \
	RegisterConfigFloat(#section "." #element, s_pEccoConfig.section.element)
#define GET_VALUE_OR_STRING(section, element, def) \
	GET_VALUE_RAW(section, element, def) \
	RegisterConfigString(#section "." #element, s_pEccoConfig.section.element)
#define GET_VALUE_OR_BOOL(section, element, def) \
	GET_VALUE_RAW(section, element, def) \
	RegisterConfigBool(#section "." #element, s_pEccoConfig.section.element)
#define GET_VALUE_OR_VECTOR_STRING(section, element) \
	do { \
		auto arr = toml["Ecco"][#section][#element].as_array(); \
		if (arr) { \
			for (auto iter = arr->begin(); iter != arr->end(); iter++) { \
				auto str = (*iter).value_or(""); \
				s_pEccoConfig.section.element.push_back(str); \
			} \
		} \
		RegisterConfigVectorString(#section "." #element, s_pEccoConfig.section.element); \
	} while(0)

#define GET_ROOT_VALUE_RAW(element, def) \
	s_pEccoConfig.element = toml["Ecco"][#element].value_or(def);
#define GET_ROOT_VALUE_OR_INT(element, def) \
	GET_ROOT_VALUE_RAW(element, def) \
	RegisterConfigInt(#element, s_pEccoConfig.element)
#define GET_ROOT_VALUE_OR_FLOAT(element, def) \
	GET_ROOT_VALUE_RAW(element, def) \
	RegisterConfigFloat(#element, s_pEccoConfig.element)
#define GET_ROOT_VALUE_OR_STRING(element, def) \
	GET_ROOT_VALUE_RAW(element, def) \
	RegisterConfigString(#element, s_pEccoConfig.element)

		GET_VALUE_OR_INT(Hud, ShowMoneyHUD, 1);
		GET_VALUE_OR_FLOAT(Hud, MainHudPosX, 0.5f);
		GET_VALUE_OR_FLOAT(Hud, MainHudPosY, 0.9f);
		GET_VALUE_OR_FLOAT(Hud, NoticeHudPosX, 0.5f);
		GET_VALUE_OR_FLOAT(Hud, NoticeHudPosY, 0.858f);
		GET_VALUE_OR_FLOAT(Hud, RefreshHudInterv, 1.0f);
		GET_VALUE_OR_INT(Hud, MoneyChannel, 3);
		GET_VALUE_OR_INT(Hud, NotifyChannel, 4);

		GET_VALUE_OR_STRING(Path, Data, "store");
		GET_VALUE_OR_STRING(Path, BanMaps, "config/BannedMaps.txt");
		GET_VALUE_OR_STRING(Path, MoneyIcon, "misc_bdsc/dollar.spr");

		GET_VALUE_OR_UINT(Color, Positive, 0xFFFF00FF);
		GET_VALUE_OR_UINT(Color, Negative, 0xFF0000FF);
		GET_VALUE_OR_UINT(Color, Incresed, 0x00FF00FF);
		GET_VALUE_OR_UINT(Color, Decresed, 0xFF0000FF);

		GET_VALUE_OR_VECTOR_STRING(BuyMenu, OpenShopTriggers);
		GET_VALUE_OR_BOOL(BuyMenu, AllowDeathPlayerBuy, true);
		GET_VALUE_OR_BOOL(BuyMenu, ReOpenMenuAfterBuy, true);
		GET_VALUE_OR_FLOAT(BuyMenu, KeepOpenTime, 15.0f);

		GET_ROOT_VALUE_OR_FLOAT(ScoreToMoneyMultiplier, 1.0f);
		GET_ROOT_VALUE_OR_INT(StorePlayerScore, 1);
		GET_ROOT_VALUE_OR_INT(PlayerStartScore, 0);
		GET_ROOT_VALUE_OR_INT(MoneyLimitePerMap, -1);
		GET_ROOT_VALUE_OR_FLOAT(SaveDelayTime, 30.0f);
		GET_ROOT_VALUE_OR_STRING(DefaultLang, "english");
		GET_ROOT_VALUE_OR_STRING(QueryLangCVar, "");
		GET_ROOT_VALUE_OR_INT(PrecacheOffset, 4);

		// load banned maps
		std::filesystem::path banmapspath(EccoMetaUtility::GetGameDir());
		banmapspath.append("addons/ecco/");
		banmapspath.append(s_pEccoConfig.Path.BanMaps);
		std::ifstream banmaps(banmapspath);
		std::string map;
		while (std::getline(banmaps, map)) {
			s_aryBannedMaps.push_back(map);
		}
	}
	catch (const toml::parse_error& err){
		auto& src = err.source();
		LOG_CONSOLE(PLID, "Pharse file %s \n\tat line %d, column %d\n\twith error: %s", src.path.get()->c_str(), src.begin.line, src.begin.column, err.what());
		return false;
	}
	return true;
}

bool SetConfigValue(const std::string& path, const std::string& value) {
	auto it = s_configMap.find(path);
	if (it == s_configMap.end()) {
		return false;
	}
	return it->second.set(value);
}

std::string GetConfigValue(const std::string& path) {
	auto it = s_configMap.find(path);
	if (it == s_configMap.end()) {
		return "";
	}
	return it->second.get();
}

std::vector<std::string> GetAllConfigPaths() {
	std::vector<std::string> paths;
	for (const auto& pair : s_configMap) {
		paths.push_back(pair.first);
	}
	return paths;
}

bool SaveEccoConfig() {
	try {
		toml::table ecco_table;
		std::map<std::string, toml::table> section_tables;
		
		for (const auto& [path, config_value] : s_configMap) {
			size_t dot_pos = path.find('.');
			if (dot_pos == std::string::npos) {
				continue;
			}
			
			std::string section = path.substr(0, dot_pos);
			std::string element = path.substr(dot_pos + 1);
			
			if (section_tables.find(section) == section_tables.end()) {
				section_tables[section] = toml::table();
			}
			
			std::string value_str = config_value.get();
			
			switch (config_value.type) {
				case ConfigValueType::UInt:
					section_tables[section].insert(element, static_cast<uint32_t>(std::stoul(value_str)));
					break;
				case ConfigValueType::Int:
					section_tables[section].insert(element, std::stoi(value_str));
					break;
				case ConfigValueType::Float:
					section_tables[section].insert(element, std::stof(value_str));
					break;
				case ConfigValueType::String:
					section_tables[section].insert(element, value_str);
					break;
				case ConfigValueType::Bool:
					section_tables[section].insert(element, value_str == "true");
					break;
				case ConfigValueType::VectorString: {
					toml::array arr;
					size_t start = value_str.find('[');
					size_t end = value_str.rfind(']');
					if (start != std::string::npos && end != std::string::npos) {
						std::string content = value_str.substr(start + 1, end - start - 1);
						std::istringstream iss(content);
						std::string token;
						
						while (std::getline(iss, token, ',')) {
							size_t quote_start = token.find('"');
							size_t quote_end = token.rfind('"');
							if (quote_start != std::string::npos && quote_end != std::string::npos && quote_start < quote_end) {
								arr.push_back(token.substr(quote_start + 1, quote_end - quote_start - 1));
							}
						}
					}
					section_tables[section].insert(element, arr);
					break;
				}
			}
		}
		
		for (const auto& [section, table] : section_tables) {
			ecco_table.insert(section, table);
		}

		toml::table root_table;
		root_table.insert("Ecco", ecco_table);

		std::filesystem::path gamedir(EccoMetaUtility::GetGameDir());
		gamedir.append("addons/ecco/Config.toml");
		
		std::ofstream file(gamedir);
		file << root_table;
		file.close();

		return true;
	}
	catch (const std::exception& e) {
		LOG_CONSOLE(PLID, "Failed to save config: %s", e.what());
		return false;
	}
}

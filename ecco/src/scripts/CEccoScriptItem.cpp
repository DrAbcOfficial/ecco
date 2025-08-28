#include <filesystem>
#include <tomlplusplus/toml.hpp>


#include <extdll.h>
#include <meta_api.h>
#include "meta_utility.h"

#include "scripts/CEccoScriptItem.h"

CEccoScriptItem::CEccoScriptItem(std::string path){
	try {
		auto toml = toml::parse_file(path);
		m_szName = toml["Ecco"]["name"].value_or("");
		auto pos = m_szName.find_last_of('.');
		m_szId = (pos == std::string::npos) ? m_szName : m_szName.substr(pos + 1);

		m_iFlags = toml["Ecco"]["flags"].value_or(0);
		m_iCost = toml["Ecco"]["cost"].value_or(0);
		m_szScripts = toml["Ecco"]["script"].value_or("");

		auto precache = toml["Ecco"]["precaches"].as_array();
		if (precache) {
			for (auto iter = precache->begin(); iter != precache->end(); iter++) {
				std::filesystem::path path = (*iter).value_or("");
				if (!path.empty()) {
					auto ext = path.extension();
					if (ext == ".spr")
						path = "sprites" / path;
					else if (ext == ".mdl")
						path = "models" / path;
					else
						LOG_CONSOLE(PLID, "Unkown extension: %s, but add it to precache list and try to precache.", ext);
					m_aryPrecaches.push_back(path.string());
				}
			}
		}

		auto sound_precache = toml["Ecco"]["sound_precaches"].as_array();
		if (sound_precache) {
			for (auto iter = sound_precache->begin(); iter != sound_precache->end(); iter++) {
				m_arySoundPrecaches.push_back((*iter).value_or(""));
			}
		}

		auto other_precache = toml["Ecco"]["other_precaches"].as_array();
		if (other_precache) {
			for (auto iter = other_precache->begin(); iter != other_precache->end(); iter++) {
				m_aryOtherPrecaches.push_back((*iter).value_or(""));
			}
		}
	}
	catch (const toml::parse_error& err) {
		auto& src = err.source();
		LOG_CONSOLE(PLID, "Pharse file %s \n\tat line %d, column %d\n\twith error: %s", src.path.get()->c_str(), src.begin.line, src.begin.column, err.what());
	}
}

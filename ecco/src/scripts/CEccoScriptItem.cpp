#include <tomlplusplus/toml.hpp>

#include <extdll.h>
#include <meta_api.h>

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

		auto precache = toml["Ecoo"]["precaches"].as_array();
		if (precache) {
			for (auto iter = precache->begin(); iter != precache->end(); iter++) {
				m_aryPrecaches.push_back((*iter).value_or(""));
			}
		}

		auto sound_precache = toml["Ecoo"]["sound_precaches"].as_array();
		if (sound_precache) {
			for (auto iter = sound_precache->begin(); iter != sound_precache->end(); iter++) {
				m_arySoundPrecaches.push_back((*iter).value_or(""));
			}
		}

		auto other_precache = toml["Ecoo"]["other_precaches"].as_array();
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

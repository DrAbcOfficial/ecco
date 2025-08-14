#include <tomlplusplus/toml.hpp>
#include <filesystem>

#include <extdll.h>
#include <meta_api.h>

#include "meta_utility.h"
#include "CConfig.h"

static CEccoConfig s_pEccoConfig;

CEccoConfig* GetEccoConfig(){
	return &s_pEccoConfig;
}

bool LoadEccoConfig(){
	std::filesystem::path gamedir(GetGameDir());
	gamedir.append("addons/ecco/Config.toml");
	try{
		auto toml = toml::parse_file(gamedir.string());
#define GET_VALUE_OR(section, element, def)s_pEccoConfig.section.element=toml["Ecco"][#section][#element].value_or(def)
#define GET_ROOT_VALUE_OR(element, def)s_pEccoConfig.element=toml["Ecco"][#element].value_or(def)
		GET_VALUE_OR(Hud, ShowMoneyHUD, 1);
		GET_VALUE_OR(Hud, MainHudPosX, 0.5f);
		GET_VALUE_OR(Hud, MainHudPosY, 0.9f);
		GET_VALUE_OR(Hud, NoticeHudPosX, 0.5f);
		GET_VALUE_OR(Hud, NoticeHudPosY, 0.858f);
		GET_VALUE_OR(Hud, RefreshHudInterv, 1.0f);
		GET_VALUE_OR(Hud, MoneyChannel, 3);
		GET_VALUE_OR(Hud, NotifyChannel, 4);
		GET_VALUE_OR(Hud, IgnoreHUDZeroChange, true);

		GET_VALUE_OR(Path, Data, "store");
		GET_VALUE_OR(Path, BanMaps, "config/BannedMaps.txt");
		GET_VALUE_OR(Path, MoneyIcon, "misc_bdsc/dollar.spr");

		GET_VALUE_OR(Color, Positive, 0xFFFF00FF);
		GET_VALUE_OR(Color, Negative, 0xFF0000FF);
		GET_VALUE_OR(Color, Incresed, 0x00FF00FF);
		GET_VALUE_OR(Color, Decresed, 0xFF0000FF);

		GET_VALUE_OR(BuyMenu, AllowIgnoreBuyPrefix, false);
		auto shoptrigger = toml["Ecco"]["BuyMenu"]["OpenShopTriggers"].as_array();
		for (auto iter = shoptrigger->begin(); iter != shoptrigger->end(); iter++) {
			auto str = (*iter).value_or("");
			s_pEccoConfig.BuyMenu.OpenShopTriggers.push_back(str);
		}
		GET_VALUE_OR(BuyMenu, UseBlurMatchForCommand, true);
		GET_VALUE_OR(BuyMenu, AllowDeathPlayerBuy, true);
		GET_VALUE_OR(BuyMenu, AllowDuplicatedBuy, true);
		GET_VALUE_OR(BuyMenu, GenerateDuplicated, true);

		GET_ROOT_VALUE_OR(ScoreToMoneyMultiplier, 1.0f);
		GET_ROOT_VALUE_OR(StorePlayerScore, 1);
		GET_ROOT_VALUE_OR(PlayerStartScore, 0);
		GET_ROOT_VALUE_OR(MoneyLimitePerMap, -1);

		auto allow = toml["Ecco"]["AllowLanguages"].as_array();
		for (auto iter = allow->begin(); iter != allow->end(); iter++) {
			auto str = (*iter).value_or("");
			s_pEccoConfig.AllowLanguages.push_back(str);
		}
	}
	catch (const toml::parse_error& err){
		auto& src = err.source();
		LOG_CONSOLE(PLID, "Pharse file %s \n\tat line %d, column %d\n\twith error: %s", src.path.get()->c_str(), src.begin.line, src.begin.column, err.what());
		return false;
	}
	return true;
}

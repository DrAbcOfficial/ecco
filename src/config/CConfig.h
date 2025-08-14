#pragma once
#include <string>
#include <vector>

struct CEccoConfigHud {
	//0 为不显示
	//1 为显示全部
	//2 为仅显示金钱不显示金钱变化
	//3 为仅显示金钱变化不显示金钱
	//显示金钱 HUD 的方式
	int ShowMoneyHUD;
	//金钱HUD显示位置
	//范围（-1，1）
	//-1和0代表居中
	float MainHudPosX;
	float MainHudPosY;

	//加减HUD显示位置
	//范围（-1，1）
	//-1和0代表居中
	float NoticeHudPosX;
	float NoticeHudPosY;
	//刷新HUD间隔
	float RefreshHudInterv;

	//金钱变更HUD频道
	int NotifyChannel;
	//金钱HUD频道
	int MoneyChannel;
};

struct CEccoConfigPath {
	//数据储存位置
	std::string Data;
	//禁用地图文件路径
	std::string BanMaps;
	//金币图标
	std::string MoneyIcon;
};

struct CEccoConfigColor {
	uint32_t Positive;
	uint32_t Negative;
	uint32_t Incresed;
	uint32_t Decresed;
};

struct CEccoConfigBuyMenu {
	//允许无视Trigger前缀
	bool AllowIgnoreBuyPrefix;
	//购买指令Trigger
	std::vector<std::string> OpenShopTriggers;
	//命令直接购买物品时是否使用模糊匹配
	bool UseBlurMatchForCommand;
	//是否允许死亡玩家购买
	bool AllowDeathPlayerBuy;
	//购买后重新打开菜单
	bool ReOpenMenuAfterBuy;
};
struct CEccoConfig{
	//Hud
	CEccoConfigHud Hud;
	//Path
	CEccoConfigPath Path;
	//Color
	CEccoConfigColor Color;
	//BuyMenu
	CEccoConfigBuyMenu BuyMenu;

	//玩家获得的分数转化为金钱的倍数
	float ScoreToMoneyMultiplier;

	//是否储存玩家金钱
	//0 不储存
	//1 只有系列地图储存
	//2 储存
	int StorePlayerScore;

	//若不储存金钱，设置玩家起始资金
	int	PlayerStartScore;

	//单张地图最大可获得金钱数目
	//小于0为不限制
	int MoneyLimitePerMap;

	//默认的语言
	std::string DefaultLang;
};

extern inline CEccoConfig* GetEccoConfig();
extern bool LoadEccoConfig();
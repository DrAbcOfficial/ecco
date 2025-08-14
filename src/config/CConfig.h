#pragma once
#include <string>
#include <vector>

struct CEccoConfigHud {
	//0 Ϊ����ʾ
	//1 Ϊ��ʾȫ��
	//2 Ϊ����ʾ��Ǯ����ʾ��Ǯ�仯
	//3 Ϊ����ʾ��Ǯ�仯����ʾ��Ǯ
	//��ʾ��Ǯ HUD �ķ�ʽ
	int ShowMoneyHUD;
	//��ǮHUD��ʾλ��
	//��Χ��-1��1��
	//-1��0�������
	float MainHudPosX;
	float MainHudPosY;

	//�Ӽ�HUD��ʾλ��
	//��Χ��-1��1��
	//-1��0�������
	float NoticeHudPosX;
	float NoticeHudPosY;
	//ˢ��HUD���
	float RefreshHudInterv;

	//��Ǯ���HUDƵ��
	int NotifyChannel;
	//��ǮHUDƵ��
	int MoneyChannel;
};

struct CEccoConfigPath {
	//���ݴ���λ��
	std::string Data;
	//���õ�ͼ�ļ�·��
	std::string BanMaps;
	//���ͼ��
	std::string MoneyIcon;
};

struct CEccoConfigColor {
	uint32_t Positive;
	uint32_t Negative;
	uint32_t Incresed;
	uint32_t Decresed;
};

struct CEccoConfigBuyMenu {
	//��������Triggerǰ׺
	bool AllowIgnoreBuyPrefix;
	//����ָ��Trigger
	std::vector<std::string> OpenShopTriggers;
	//����ֱ�ӹ�����Ʒʱ�Ƿ�ʹ��ģ��ƥ��
	bool UseBlurMatchForCommand;
	//�Ƿ�����������ҹ���
	bool AllowDeathPlayerBuy;
	//��������´򿪲˵�
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

	//��һ�õķ���ת��Ϊ��Ǯ�ı���
	float ScoreToMoneyMultiplier;

	//�Ƿ񴢴���ҽ�Ǯ
	//0 ������
	//1 ֻ��ϵ�е�ͼ����
	//2 ����
	int StorePlayerScore;

	//���������Ǯ�����������ʼ�ʽ�
	int	PlayerStartScore;

	//���ŵ�ͼ���ɻ�ý�Ǯ��Ŀ
	//С��0Ϊ������
	int MoneyLimitePerMap;

	//Ĭ�ϵ�����
	std::string DefaultLang;
};

extern inline CEccoConfig* GetEccoConfig();
extern bool LoadEccoConfig();
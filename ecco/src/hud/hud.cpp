#include <unordered_map>
#include <array>
#include <cmath>

#include "CHudNumber.h"

#include "Scheduler.h"
#include "meta_utility.h"

#include "config/CConfig.h"
#include "storage/Storage.h"

#include "hud.h"

static CHudNumber s_pCreditDisplay;
static CHudNumber s_pNoticeDIsplay;
static std::array<float, 32> s_aryPlayerLastCredit{};
static std::unordered_map<edict_t*, ScheduledFunction> s_mapHudUpdateTimers;

using namespace EccoMetaUtility;

void InitHud(){
	auto config = GetEccoConfig();
	s_pCreditDisplay.m_iChannel = config->Hud.MoneyChannel;
	s_pCreditDisplay.m_fX = config->Hud.MainHudPosX;
	s_pCreditDisplay.m_fY = config->Hud.MainHudPosY;
	s_pCreditDisplay.m_bitsFlags = HUD_ELEM_SCR_CENTER_X | HUD_ELEM_DEFAULT_ALPHA | HUD_NUM_NEGATIVE_NUMBERS;
	s_pCreditDisplay.m_iDefDigits = 1;
	s_pCreditDisplay.m_iMaxDigits = 12;
	s_pCreditDisplay.m_flHoldTime = INFINITY;
	s_pCreditDisplay.m_szSpriteName = config->Path.MoneyIcon;

	s_pNoticeDIsplay.m_iChannel = config->Hud.NotifyChannel;
	s_pNoticeDIsplay.m_fX = config->Hud.NoticeHudPosX;
	s_pNoticeDIsplay.m_fY = config->Hud.NoticeHudPosY;
	s_pNoticeDIsplay.m_bitsFlags = HUD_ELEM_SCR_CENTER_X | HUD_ELEM_DEFAULT_ALPHA | HUD_NUM_PLUS_SIGN | HUD_NUM_NEGATIVE_NUMBERS;
	s_pNoticeDIsplay.m_flFadeinTime = 0.15f;
	s_pNoticeDIsplay.m_flHoldTime = 1.0f;
	s_pNoticeDIsplay.m_flFadeoutTime = 0.15f;
	s_pNoticeDIsplay.m_iDefDigits = 1;
	s_pNoticeDIsplay.m_iMaxDigits = 8;
}

void ResetHud() {
	s_aryPlayerLastCredit = {};
}

void AddHudUpdateTimer(edict_t* pent){
	if (!pent)
		return;
	if(s_mapHudUpdateTimers.find(pent) != s_mapHudUpdateTimers.end())
		return; // already has a timer
	auto t = g_Scheduler.SetInterval(
		[pent]() {
			if(FNullEnt(pent))
				return;
			if (!IsValidPlayer(pent))
				return;
			CPlayerStorageItem* pItem = GetPlayerStorageItem(pent);
			if (pItem) {
				int index = ENTINDEX(pent) - 1;
				float last_credits = s_aryPlayerLastCredit[index];
				float credits = (float)pItem->GetCredits();
				if (last_credits == credits)
					return; // no change, skip update

				int showhud = GetEccoConfig()->Hud.ShowMoneyHUD;
				if (showhud <= 0)
					return;

				s_pNoticeDIsplay.m_iColor1 = last_credits < credits ? GetEccoConfig()->Color.Incresed : GetEccoConfig()->Color.Decresed;
				s_pCreditDisplay.m_iColor1 = credits < 0 ? GetEccoConfig()->Color.Negative : GetEccoConfig()->Color.Positive;

				if (showhud != 3) {
					if (last_credits == 0.0f && credits != 0.0f)
						s_pCreditDisplay.Send(pent, credits);

					else {
						if (std::signbit(last_credits) != std::signbit(credits))
							s_pCreditDisplay.Send(pent, credits);
						else
							s_pCreditDisplay.Update(pent, credits);
					}
				}

				if(showhud != 2)
					s_pNoticeDIsplay.Send(pent, credits - last_credits);
				s_aryPlayerLastCredit[index] = credits;
			}
		},
		GetEccoConfig()->Hud.RefreshHudInterv,
		-1 // infinite calls
	);
	s_mapHudUpdateTimers[pent] = t;
}

void RemoveHudUpdateTimer(edict_t* pent){
	if (!pent)
		return;
	auto it = s_mapHudUpdateTimers.find(pent);
	if (it != s_mapHudUpdateTimers.end()) {
		g_Scheduler.RemoveTimer(it->second);
		s_mapHudUpdateTimers.erase(it);
	}
	int index = ENTINDEX(pent) - 1;
	if (index >= 0 && (uint32)index < s_aryPlayerLastCredit.size())
		s_aryPlayerLastCredit[index] = 0.0f;
}

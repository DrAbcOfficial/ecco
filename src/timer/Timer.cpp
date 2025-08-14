#include <map>

#include "storage/Storage.h"
#include "config/CConfig.h"
#include "timer/Timer.h"

#include "Scheduler.h"

static std::map<edict_t*, ScheduledFunction> s_mapPlayerScoreToCreditsTimer;

void AddPlayerScoreToCreditsTimer(edict_t* pent){
	if (!pent)
		return;
	// already has a timer
	if(s_mapPlayerScoreToCreditsTimer.find(pent) != s_mapPlayerScoreToCreditsTimer.end())
		return;
	auto t = g_Scheduler.SetInterval(
		[pent]() {
			CPlayerStorageItem* pItem = GetPlayerStorageItem(pent);
			if (pItem)
				pItem->ScoreToCredits(pent->v.frags);
		},
		0.7f,
		-1 // infinite calls
	);
	s_mapPlayerScoreToCreditsTimer[pent] = t;
}

void RemovePlayerScoreToCreditsTimer(edict_t* pent){
	if(!pent)
		return;
	auto it = s_mapPlayerScoreToCreditsTimer.find(pent);
	if(it != s_mapPlayerScoreToCreditsTimer.end()){
		g_Scheduler.RemoveTimer(it->second);
		s_mapPlayerScoreToCreditsTimer.erase(it);
	}
}

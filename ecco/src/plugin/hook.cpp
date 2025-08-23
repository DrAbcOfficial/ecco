#include <vector>
#include "api/HookDef.h"


std::vector<Hook_OnPlayerCreditsChanged> hooks_OnPlayerCreditsChanged;
std::vector<Hook_OnPlayerScoreToCredits> hooks_OnPlayerScoreToCredits;
std::vector<Hook_OnMenuExcuted> hooks_OnMenuExcuted;

void Call_OnPlayerCreditsChanged(IPlayerStorageItem* player, int oldCredits, int& credits){
	for(auto hook : hooks_OnPlayerCreditsChanged){
		hook(player, oldCredits, credits);
	}
}

void Call_OnPlayerScoreToCredits(IPlayerStorageItem* player, int score, int& added){
	for(auto hook : hooks_OnPlayerScoreToCredits){
		hook(player, score, added);
	}
}

void Call_OnMenuExcuted(IEccoBaseExcutor* menu, IPlayerStorageItem* player){
	for(auto hook : hooks_OnMenuExcuted){
		hook(menu, player);
	}
}

void AddHook(Hook_Type type, void* func){
	switch(type){
		case Hook_Type::ON_PLAYER_CREDITS_CHANGED:
			hooks_OnPlayerCreditsChanged.push_back((Hook_OnPlayerCreditsChanged)func);
			break;
		case Hook_Type::ON_PLAYER_SCORE_TO_CREDITS:
			hooks_OnPlayerScoreToCredits.push_back((Hook_OnPlayerScoreToCredits)func);
			break;
		case Hook_Type::ON_MENU_EXCUTE:
			hooks_OnMenuExcuted.push_back((Hook_OnMenuExcuted)func);
			break;
	}
}

void RemoveHook(Hook_Type type, void* func){
	switch(type){
		case Hook_Type::ON_PLAYER_CREDITS_CHANGED:
			hooks_OnPlayerCreditsChanged.erase(std::remove(hooks_OnPlayerCreditsChanged.begin(), hooks_OnPlayerCreditsChanged.end(), (Hook_OnPlayerCreditsChanged)func), hooks_OnPlayerCreditsChanged.end());
			break;
		case Hook_Type::ON_PLAYER_SCORE_TO_CREDITS:
			hooks_OnPlayerScoreToCredits.erase(std::remove(hooks_OnPlayerScoreToCredits.begin(), hooks_OnPlayerScoreToCredits.end(), (Hook_OnPlayerScoreToCredits)func), hooks_OnPlayerScoreToCredits.end());
			break;
		case Hook_Type::ON_MENU_EXCUTE:
			hooks_OnMenuExcuted.erase(std::remove(hooks_OnMenuExcuted.begin(), hooks_OnMenuExcuted.end(), (Hook_OnMenuExcuted)func), hooks_OnMenuExcuted.end());
			break;
	}
}

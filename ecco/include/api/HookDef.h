#pragma once
#include "IPlayerStorageItem.h"
#include "IEccoBaseExcutor.h"

enum class Hook_Type {
	ON_PLAYER_CREDITS_CHANGED,
	ON_PLAYER_SCORE_TO_CREDITS,
	ON_MENU_EXCUTE
};

typedef void (*Hook_OnPlayerCreditsChanged)(IPlayerStorageItem* player, int oldCredits, int &credits);
typedef void (*Hook_OnPlayerScoreToCredits)(IPlayerStorageItem* player, int score, int& added);
typedef void (*Hook_OnMenuExcuted)(IEccoBaseExcutor* menu, IPlayerStorageItem* player);
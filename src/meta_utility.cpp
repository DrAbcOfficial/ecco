#include "meta_utility.h"
#include <meta_api.h>

bool IsValidPlayer(edict_t* pentPlayer) {
	return pentPlayer && (pentPlayer->v.flags & FL_CLIENT) != 0 && (pentPlayer->v.flags & FL_PROXY) == 0 && STRING(pentPlayer->v.netname)[0] != '\0';
}
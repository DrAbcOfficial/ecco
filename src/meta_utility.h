#pragma once
#include <extdll.h>
// get a player index as a bitfield index
#define PLAYER_BIT(edt) (1 << (ENTINDEX(edt) % 32))
extern bool IsValidPlayer(edict_t* pentPlayer);
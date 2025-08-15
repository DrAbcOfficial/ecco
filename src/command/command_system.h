#pragma once
#include <extdll.h>

extern bool ClientCommandHandler(edict_t* caller);
extern void RegistServerCommandToEngine();
extern bool ClientSayCommandHandler(edict_t* caller);
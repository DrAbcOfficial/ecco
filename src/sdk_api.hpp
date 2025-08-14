#pragma once
#ifndef __ECCO_SVEN_API
#define __ECCO_SVEN_API
#ifdef _GAME_SVENCOOP
#ifdef _WIN32
#include "windows/private_api.h"
#elif __linux__
#include "linux/private_api.h"
#else
#error Not supported in this OS, are you playing game in MacOS?
#endif // WIN32
#endif // _GAME_SVENCOOP
#endif // __ECCO_SVEN_API
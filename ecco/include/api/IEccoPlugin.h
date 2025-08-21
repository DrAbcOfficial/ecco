#pragma once
#include "IEccoScriptSystem.h"
#include "IEccoFuncs.h"
#include "IPlayerStorageItem.h"
#include "IEccoBaseExcutor.h"
#include <extdll.h>
#include <meta_api.h>

class IEccoPlugin {
public:
	virtual const char* GetName() const = 0;
	virtual const char* GetDescription() const = 0;
	virtual const char* GetVersion() const = 0;
	virtual const char* GetAuthor() const = 0;

	virtual void Initialize(IEccoScriptSystem* script_system, IEccoFuncs* ecco_funcs,
		enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals,
		meta_globals_t* penMetaGlobals, gamedll_funcs_t* penGamedllFuncs, mutil_funcs_t* penMetaUtilFuncs) = 0;
	virtual void Shutdown() = 0;
};

#ifdef _WIN32
#define ECCO_API extern "C" __declspec(dllexport)
#else
#define ECCO_API extern "C" __attribute__((visibility("default")))
#endif

ECCO_API IEccoPlugin* GetEccoPluginInstance();
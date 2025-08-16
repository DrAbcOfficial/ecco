#include "CEccoScriptSystem.h"

static chaiscript::ChaiScript g_ScriptEngine;

chaiscript::ChaiScript* CEccoScriptSystem::GetScriptEngine() const{
	return &g_ScriptEngine;
}

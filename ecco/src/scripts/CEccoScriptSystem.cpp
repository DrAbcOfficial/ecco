#include <chaiscript/chaiscript.hpp>

#include "CEccoScriptSystem.h"

static chaiscript::ChaiScript g_ScriptEngine;

void* CEccoScriptSystem::GetScriptEngine() const{
	return &g_ScriptEngine;
}

#define DECLARE_ADD_METHOD(fn)  \
void CEccoScriptSystem::AddMethod(const char* szName, fn pfnMethod){ \
	g_ScriptEngine.add(chaiscript::fun(pfnMethod), szName); \
}

#define DECLARE_METHOD_GROUP(sym) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##s1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##s2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##s3) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1s1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1s2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1s3) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##n1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##n2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##n3) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1n1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1n2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1n3) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##fp1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##fp2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##fp3) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1fp1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1fp2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1fp3) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##f1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##f2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##f3) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1f1) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1f2) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##p1f3) \
	DECLARE_ADD_METHOD(fnScriptMethods##sym##n1fp3)


DECLARE_METHOD_GROUP(b)
DECLARE_METHOD_GROUP(p)
DECLARE_METHOD_GROUP(s)
DECLARE_METHOD_GROUP(n)
DECLARE_METHOD_GROUP(f)
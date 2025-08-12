#include <extdll.h>
#include "angelscript.hpp"
#include <meta_api.h>

angelhook_t g_AngelHook;

void RegisterAngelScriptMethods(){
	ASEXT_RegisterDocInitCallback([](CASDocumentation* pASDoc) {
		
	});
}

void RegisterAngelScriptHooks(){

}
#undef CREATE_AS_HOOK
#ifdef _GAME_SVENCOOP
#include <extdll.h>
#include <meta_api.h>
#include "angelscript/angelscript.hpp"

angelhook_t g_AngelHook;

void RegisterAngelScriptMethods(){
	ASEXT_RegisterDocInitCallback([](CASDocumentation* pASDoc) {
		
	});
}

void RegisterAngelScriptHooks(){

}
#endif // _GAME_SVENCOOP
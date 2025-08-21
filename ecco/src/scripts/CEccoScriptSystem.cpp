#include <filesystem>

#include "tcl_dynamic.h"
#undef DLLEXPORT
#undef DLLIMPORT
#undef CRTIMPORT

#include "CEccoScriptSystem.h"

#include <extdll.h>
#include <meta_api.h>
#include "meta_utility.h"

static Tcl_Interp* s_pTclinterp = nullptr;
static char s_szLibpath[MAX_PATH] = { 0 };
static char s_szPkgpath[MAX_PATH] = { 0 };

CEccoScriptSystem::CEccoScriptSystem(){
	if (s_pTclinterp)
		Tcl_DeleteInterp(s_pTclinterp);
	s_pTclinterp = Tcl_CreateInterp();

	auto tclPath = std::filesystem::path(EccoMetaUtility::GetGameDir());
	tclPath.append("addons/ecco/tcl");
	strncpy(s_szLibpath, (tclPath.string() + "/lib").c_str(), MAX_PATH);
	strncpy(s_szPkgpath, (tclPath.string() + "/pkg").c_str(), MAX_PATH);

	Tcl_SetVar(s_pTclinterp, "tcl_library", s_szLibpath, TCL_GLOBAL_ONLY);
	Tcl_SetVar(s_pTclinterp, "tcl_pkgPath", s_szPkgpath, TCL_GLOBAL_ONLY);
	Tcl_SetVar(s_pTclinterp, "tcl_interactive", "0", TCL_GLOBAL_ONLY);
	if (Tcl_Init(s_pTclinterp) == TCL_ERROR) {
		LOG_ERROR(PLID, "Tcl init failed!\n%s", Tcl_GetStringResult(s_pTclinterp));
		return;
	}
	char path[MAX_PATH];
	snprintf(path, MAX_PATH, "{%s}\n{%s}", s_szLibpath, s_szPkgpath);
	Tcl_SetVar(s_pTclinterp, "auto_path", path, TCL_GLOBAL_ONLY);
	if (Tcl_SetSystemEncoding(s_pTclinterp, "utf-8") == TCL_ERROR) {
		LOG_ERROR(PLID, "Tcl set utf-8 encoding failed!\n%s", Tcl_GetStringResult(s_pTclinterp));
	}
}

void CEccoScriptSystem::CreateCommand(const char* name, fnFunc callback){
	using cmd_pair_t = struct cmd_pair_s{
		CEccoScriptSystem* pThis;
		fnFunc* callback;
	};
	cmd_pair_t* pair = new cmd_pair_t{ this, &callback };
	Tcl_CreateCommand(s_pTclinterp, name, [](ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[]) {
		auto pair = static_cast<cmd_pair_t*>(clientData);
		auto ret = (*pair->callback)(pair->pThis, argc, argv);
		return (int)ret;
	}, pair, nullptr);
}
CEccoScriptSystem::Result CEccoScriptSystem::Eval(const char* content){
	auto result = Tcl_Eval(s_pTclinterp, content);
	if (result == TCL_ERROR) {
		const char* errorInfo = Tcl_GetVar(s_pTclinterp, "errorInfo", TCL_GLOBAL_ONLY);
		const char* errorCode = Tcl_GetVar(s_pTclinterp, "errorCode", TCL_GLOBAL_ONLY);
		LOG_DEVELOPER(PLID, "Error when eval script, code: %s\n%s\n", errorCode, errorInfo);
	}
	return (Result)result;
}
void CEccoScriptSystem::ThrowError(const char* code, const char* msg){
	Tcl_SetErrorCode(s_pTclinterp, code, nullptr);
	Tcl_AddErrorInfo(s_pTclinterp, msg);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewObject(){
	return Tcl_NewObj();
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewIntObject(int value){
	return Tcl_NewIntObj(value);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewStringObject(const char* value, size_t len){
	return Tcl_NewStringObj(value, len);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewDoubleObject(double value){
	return Tcl_NewDoubleObj(value);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewDictObject(){
	return Tcl_NewDictObj();
}
void CEccoScriptSystem::SetDictValue(Object* dict, const char* key, Object* value){
	Tcl_DictObjPut(s_pTclinterp, static_cast<Tcl_Obj*>(dict), Tcl_NewStringObj(key, -1), static_cast<Tcl_Obj*>(value));
}
IEccoScriptSystem::Object* CEccoScriptSystem::GetDictValue(Object* dict, const char* key){
	Tcl_Obj* value = nullptr;
	int result = Tcl_DictObjGet(s_pTclinterp, static_cast<Tcl_Obj*>(dict), Tcl_NewStringObj(key, -1), &value);
	if (result != TCL_OK || value == nullptr) {
		return nullptr;
	}
	return value;
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewBooleanObject(bool value){
	return Tcl_NewBooleanObj(value);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewListObject(size_t size, Object** list) {
	return Tcl_NewListObj(static_cast<int>(size), reinterpret_cast<Tcl_Obj**>(list));
}
IEccoScriptSystem::Object* CEccoScriptSystem::GetListObject(Object* list, size_t index){
	Tcl_Obj* elem = nullptr;
	int result = Tcl_ListObjIndex(s_pTclinterp, static_cast<Tcl_Obj*>(list), static_cast<Tcl_Size>(index), &elem);
	if (result != TCL_OK || elem == nullptr) {
		return nullptr;
	}
	return elem;
}

void CEccoScriptSystem::AppendListObject(Object* list, Object* obj){
	Tcl_ListObjAppendElement(s_pTclinterp, static_cast<Tcl_Obj*>(list), static_cast<Tcl_Obj*>(obj));
}

void CEccoScriptSystem::SetListObject(Object* list, size_t index, Object* obj){
	Tcl_Size listLen = 0;
	Tcl_ListObjLength(s_pTclinterp, static_cast<Tcl_Obj*>(list), &listLen);
	if (index >= static_cast<size_t>(listLen)) {
		for (Tcl_Size i = listLen; i <= static_cast<Tcl_Size>(index); ++i) {
			Tcl_ListObjAppendElement(s_pTclinterp, static_cast<Tcl_Obj*>(list), Tcl_NewObj());
		}
	}
	Tcl_Obj* objs[1] = { static_cast<Tcl_Obj*>(obj) };
	Tcl_ListObjReplace(s_pTclinterp, static_cast<Tcl_Obj*>(list), static_cast<Tcl_Size>(index), 1, 1, objs);
}

size_t CEccoScriptSystem::GetListSize(Object* list){
	Tcl_Size size = 0;
	Tcl_ListObjLength(s_pTclinterp, static_cast<Tcl_Obj*>(list), &size);
	return size;
}

void CEccoScriptSystem::SetObjectResult(Object* object){
	return Tcl_SetObjResult(s_pTclinterp, static_cast<Tcl_Obj*>(object));
}
void CEccoScriptSystem::SetValue(const char* name, Object* value){
	Tcl_SetVar2Ex(s_pTclinterp, name, nullptr, static_cast<Tcl_Obj*>(value), TCL_GLOBAL_ONLY);
}
void CEccoScriptSystem::UnsetValue(const char* name){
	Tcl_UnsetVar2(s_pTclinterp, name, nullptr, TCL_GLOBAL_ONLY);
}
void* CEccoScriptSystem::GetScriptEngine() const{
	return s_pTclinterp;
}

void CEccoScriptSystem::ResetEnviroment(){
	Tcl_ResetResult(s_pTclinterp);
}
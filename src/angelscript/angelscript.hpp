#pragma once
#ifdef _GAME_SVENCOOP
#ifndef __ANGELSCRIPT_API_HEADER
#define __ANGELSCRIPT_API_HEADER

#include "asext_api.h"
#include "angelscript/angelscriptlib.h"

#pragma region Template
template <typename T>
void RegisteRefObject(CASDocumentation* pASDoc, const char* szName) {
	asSFuncPtr reg;
	reg = asMETHOD(T, AddRef);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "AddRef", szName, asBEHAVE_ADDREF, "void AddRef()", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, Release);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Release", szName, asBEHAVE_RELEASE, "void Release()", &reg, asCALL_THISCALL);
}
template <typename T>
void RegisteGCObject(CASDocumentation* pASDoc, const char* szName) {
	RegisteRefObject<T>(pASDoc, szName);
	asSFuncPtr reg;
	reg = asMETHOD(T, SetGCFlag);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Set GC Flag", szName, asBEHAVE_SETGCFLAG, "void SetGCFlag()", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, GetGCFlag);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Get GC Flag", szName, asBEHAVE_GETGCFLAG, "bool GetGCFlag() const", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, GetRefCount);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Get reference count", szName, asBEHAVE_GETREFCOUNT, "int GetRefCount() const", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, EnumReferences);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Enumerate references held by this class", szName, asBEHAVE_ENUMREFS, "void EnumReferences(int& in)", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, ReleaseReferences);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Release all references held by this class", szName, asBEHAVE_RELEASEREFS, "void ReleaseReferences(int& in)", &reg, asCALL_THISCALL);
}
#pragma endregion

#pragma region Methods
extern void RegisterAngelScriptMethods();
extern void RegisterAngelScriptHooks();
#pragma endregion

#pragma region TypeDef
using angelhook_t = struct angelhook_s {

};
extern angelhook_t g_AngelHook;
#pragma endregion

#pragma region Marco
#define AS_REG_OBJMETHODEX(r, d, e, c, m, cc, mm, call) r=asMETHOD(cc,mm);ASEXT_RegisterObjectMethodEx(d,e,c,m,&r,call)
#define AS_REG_OBJMETHODPREX(r, d, e, c, m, cc, mm, pp, rr, call) r=asMETHODPR(cc,mm, pp, rr);ASEXT_RegisterObjectMethodEx(d,e,c,m,&r,call)
#define AS_CREATE_HOOK(item, des, tag, name, arg) g_AngelHook.item=ASEXT_RegisterHook(des,StopMode_CALL_ALL,2,ASFlag_MapScript|ASFlag_Plugin,tag,name,arg)
#define AS_CALL(pfn, ...) if (ASEXT_CallHook){(*ASEXT_CallHook)(g_AngelHook.pfn, 0, __VA_ARGS__);}
#pragma endregion

#endif // !__ANGELSCRIPT_API_HEADER
#endif // _GAME_SVENCOOP
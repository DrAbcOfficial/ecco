#pragma once
#include "IPlayerStorageItem.h"

// If you really don't want to include the whole chaiscript header, you can use this
#pragma region TypeDef

#define DEFINE_FUNCTION(sym, type) \
	typedef type (*fnScriptMethods##sym)(); \
	typedef type (*fnScriptMethods##sym##p1)(IPlayerStorageItem* item); \
	typedef type (*fnScriptMethods##sym##p2)(IPlayerStorageItem* item1, IPlayerStorageItem* item2); \
	typedef type (*fnScriptMethods##sym##s1)(const char* str1); \
	typedef type (*fnScriptMethods##sym##s2)(const char* str1, const char* str2); \
	typedef type (*fnScriptMethods##sym##s3)(const char* str1, const char* str2, const char* str3); \
	typedef type (*fnScriptMethods##sym##p1s1)(IPlayerStorageItem* item, const char* str1); \
	typedef type (*fnScriptMethods##sym##p1s2)(IPlayerStorageItem* item, const char* str1, const char* str2); \
	typedef type (*fnScriptMethods##sym##p1s3)(IPlayerStorageItem* item, const char* str1, const char* str2, const char* str3); \
	typedef type (*fnScriptMethods##sym##n1)(int num1); \
	typedef type (*fnScriptMethods##sym##n2)(int num1, int num2); \
	typedef type (*fnScriptMethods##sym##n3)(int num1, int num2, int num3); \
	typedef type (*fnScriptMethods##sym##p1n1)(IPlayerStorageItem* item, int num1); \
	typedef type (*fnScriptMethods##sym##p1n2)(IPlayerStorageItem* item, int num1, int num2); \
	typedef type (*fnScriptMethods##sym##p1n3)(IPlayerStorageItem* item, int num1, int num2, int num3); \
	typedef type (*fnScriptMethods##sym##fp1)(float* fl1); \
	typedef type (*fnScriptMethods##sym##fp2)(float* fl1, float* fl2); \
	typedef type (*fnScriptMethods##sym##fp3)(float* fl1, float* fl2, float* fl3); \
	typedef type (*fnScriptMethods##sym##p1fp1)(IPlayerStorageItem* item, float* fl1); \
	typedef type (*fnScriptMethods##sym##p1fp2)(IPlayerStorageItem* item, float* fl1, float* fl2); \
	typedef type (*fnScriptMethods##sym##p1fp3)(IPlayerStorageItem* item, float* fl1, float* fl2, float* fl3); \
	typedef type (*fnScriptMethods##sym##n1fp3)(int num1, float* fl1, float* fl2, float* fl3); \
	typedef type (*fnScriptMethods##sym##f1)(float fl1); \
	typedef type (*fnScriptMethods##sym##f2)(float fl1, float fl2); \
	typedef type (*fnScriptMethods##sym##f3)(float fl1, float fl2, float fl3); \
	typedef type (*fnScriptMethods##sym##p1f1)(IPlayerStorageItem* item, float fl1); \
	typedef type (*fnScriptMethods##sym##p1f2)(IPlayerStorageItem* item, float fl1, float fl2); \
	typedef type (*fnScriptMethods##sym##p1f3)(IPlayerStorageItem* item, float fl1, float fl2, float fl3); 

DEFINE_FUNCTION(b, bool)
DEFINE_FUNCTION(p, IPlayerStorageItem*)
DEFINE_FUNCTION(s, const char*)
DEFINE_FUNCTION(n, int)
DEFINE_FUNCTION(f, float)
#pragma endregion

#pragma region CLASS_HELPER
#define ADD_METHOD_BASE(func) \
	virtual void AddMethod(const char* szName, fnScriptMethods##func)

#define ADD_METHODS_ABSTRACT(sym) \
	ADD_METHOD_BASE(sym) = 0; \
	ADD_METHOD_BASE(sym##p1) = 0; \
	ADD_METHOD_BASE(sym##p2) = 0; \
	ADD_METHOD_BASE(sym##s1) = 0; \
	ADD_METHOD_BASE(sym##s2) = 0; \
	ADD_METHOD_BASE(sym##s3) = 0; \
	ADD_METHOD_BASE(sym##p1s1) = 0; \
	ADD_METHOD_BASE(sym##p1s2) = 0; \
	ADD_METHOD_BASE(sym##p1s3) = 0; \
	ADD_METHOD_BASE(sym##n1) = 0; \
	ADD_METHOD_BASE(sym##n2) = 0; \
	ADD_METHOD_BASE(sym##n3) = 0; \
	ADD_METHOD_BASE(sym##p1n1) = 0; \
	ADD_METHOD_BASE(sym##p1n2) = 0; \
	ADD_METHOD_BASE(sym##p1n3) = 0; \
	ADD_METHOD_BASE(sym##fp1) = 0; \
	ADD_METHOD_BASE(sym##fp2) = 0; \
	ADD_METHOD_BASE(sym##fp3) = 0; \
	ADD_METHOD_BASE(sym##p1fp1) = 0; \
	ADD_METHOD_BASE(sym##p1fp2) = 0; \
	ADD_METHOD_BASE(sym##p1fp3) = 0; \
	ADD_METHOD_BASE(sym##f1) = 0; \
	ADD_METHOD_BASE(sym##f2) = 0; \
	ADD_METHOD_BASE(sym##f3) = 0; \
	ADD_METHOD_BASE(sym##p1f1) = 0; \
	ADD_METHOD_BASE(sym##p1f2) = 0; \
	ADD_METHOD_BASE(sym##p1f3) = 0; \
	ADD_METHOD_BASE(sym##n1fp3) = 0;

#define ADD_METHODS(sym) \
	ADD_METHOD_BASE(sym); \
	ADD_METHOD_BASE(sym##p1); \
	ADD_METHOD_BASE(sym##p2); \
	ADD_METHOD_BASE(sym##s1); \
	ADD_METHOD_BASE(sym##s2); \
	ADD_METHOD_BASE(sym##s3); \
	ADD_METHOD_BASE(sym##p1s1); \
	ADD_METHOD_BASE(sym##p1s2); \
	ADD_METHOD_BASE(sym##p1s3); \
	ADD_METHOD_BASE(sym##n1); \
	ADD_METHOD_BASE(sym##n2); \
	ADD_METHOD_BASE(sym##n3); \
	ADD_METHOD_BASE(sym##p1n1); \
	ADD_METHOD_BASE(sym##p1n2); \
	ADD_METHOD_BASE(sym##p1n3); \
	ADD_METHOD_BASE(sym##fp1); \
	ADD_METHOD_BASE(sym##fp2); \
	ADD_METHOD_BASE(sym##fp3); \
	ADD_METHOD_BASE(sym##p1fp1); \
	ADD_METHOD_BASE(sym##p1fp2); \
	ADD_METHOD_BASE(sym##p1fp3); \
	ADD_METHOD_BASE(sym##f1); \
	ADD_METHOD_BASE(sym##f2); \
	ADD_METHOD_BASE(sym##f3); \
	ADD_METHOD_BASE(sym##p1f1); \
	ADD_METHOD_BASE(sym##p1f2); \
	ADD_METHOD_BASE(sym##p1f3); \
	ADD_METHOD_BASE(sym##n1fp3);
#pragma endregion

class IEccoScriptSystem {
public:
	ADD_METHODS_ABSTRACT(b)
	ADD_METHODS_ABSTRACT(p)
	ADD_METHODS_ABSTRACT(s)
	ADD_METHODS_ABSTRACT(n)
	ADD_METHODS_ABSTRACT(f)
	
	// if you want to include the chaiscript header
	virtual void* GetScriptEngine() const = 0;
};
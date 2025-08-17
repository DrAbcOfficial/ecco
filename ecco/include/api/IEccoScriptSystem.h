#pragma once
#include "IPlayerStorageItem.h"

// If you really don't want to include the whole chaiscript header, you can use this
#pragma region TypeDef
typedef bool (*fnScriptMethodsb)();
typedef bool (*fnScriptMethodsbp1)(IPlayerStorageItem* item);
typedef bool (*fnScriptMethodsbp2)(IPlayerStorageItem* item1, IPlayerStorageItem* item2);
typedef bool (*fnScriptMethodsbs1)(const char* str1);
typedef bool (*fnScriptMethodsbs2)(const char* str1, const char* str2);
typedef bool (*fnScriptMethodsbs3)(const char* str1, const char* str2, const char* str3);
typedef bool (*fnScriptMethodsbn1)(int num1);
typedef bool (*fnScriptMethodsbn2)(int num1, int num2);
typedef bool (*fnScriptMethodsbn3)(int num1, int num2, int num3);
typedef bool (*fnScriptMethodsbp1s1)(IPlayerStorageItem* item, const char* str1);
typedef bool (*fnScriptMethodsbp1s2)(IPlayerStorageItem* item, const char* str1, const char* str2);
typedef bool (*fnScriptMethodsbp1s3)(IPlayerStorageItem* item, const char* str1, const char* str2, const char* str3);
typedef bool (*fnScriptMethodsbp1n1)(IPlayerStorageItem* item, int num1);
typedef bool (*fnScriptMethodsbp1n2)(IPlayerStorageItem* item, int num1, int num2);
typedef bool (*fnScriptMethodsbp1n3)(IPlayerStorageItem* item, int num1, int num2, int num3);

typedef int (*fnScriptMethodsn)();
typedef int (*fnScriptMethodsnp1)(IPlayerStorageItem* item);
typedef int (*fnScriptMethodsnp2)(IPlayerStorageItem* item1, IPlayerStorageItem* item2);
typedef int (*fnScriptMethodsns1)(const char* str1);
typedef int (*fnScriptMethodsns2)(const char* str1, const char* str2);
typedef int (*fnScriptMethodsns3)(const char* str1, const char* str2, const char* str3);
typedef int (*fnScriptMethodsnn1)(int num1);
typedef int (*fnScriptMethodsnn2)(int num1, int num2);
typedef int (*fnScriptMethodsnn3)(int num1, int num2, int num3);
typedef int (*fnScriptMethodsnp1s1)(IPlayerStorageItem* item, const char* str1);
typedef int (*fnScriptMethodsnp1s2)(IPlayerStorageItem* item, const char* str1, const char* str2);
typedef int (*fnScriptMethodsnp1s3)(IPlayerStorageItem* item, const char* str1, const char* str2, const char* str3);
typedef int (*fnScriptMethodsnp1n1)(IPlayerStorageItem* item, int num1);
typedef int (*fnScriptMethodsnp1n2)(IPlayerStorageItem* item, int num1, int num2);
typedef int (*fnScriptMethodsnp1n3)(IPlayerStorageItem* item, int num1, int num2, int num3);

typedef IPlayerStorageItem* (*fnScriptMethodsp)();
typedef IPlayerStorageItem* (*fnScriptMethodspp1)(IPlayerStorageItem* item);
typedef IPlayerStorageItem* (*fnScriptMethodspp2)(IPlayerStorageItem* item1, IPlayerStorageItem* item2);
typedef IPlayerStorageItem* (*fnScriptMethodsps1)(const char* str1);
typedef IPlayerStorageItem* (*fnScriptMethodsps2)(const char* str1, const char* str2);
typedef IPlayerStorageItem* (*fnScriptMethodsps3)(const char* str1, const char* str2, const char* str3);
typedef IPlayerStorageItem* (*fnScriptMethodspn1)(int num1);
typedef IPlayerStorageItem* (*fnScriptMethodspn2)(int num1, int num2);
typedef IPlayerStorageItem* (*fnScriptMethodspn3)(int num1, int num2, int num3);
typedef IPlayerStorageItem* (*fnScriptMethodspp1s1)(IPlayerStorageItem* item, const char* str1);
typedef IPlayerStorageItem* (*fnScriptMethodspp1s2)(IPlayerStorageItem* item, const char* str1, const char* str2);
typedef IPlayerStorageItem* (*fnScriptMethodspp1s3)(IPlayerStorageItem* item, const char* str1, const char* str2, const char* str3);
typedef IPlayerStorageItem* (*fnScriptMethodspp1n1)(IPlayerStorageItem* item, int num1);
typedef IPlayerStorageItem* (*fnScriptMethodspp1n2)(IPlayerStorageItem* item, int num1, int num2);
typedef IPlayerStorageItem* (*fnScriptMethodspp1n3)(IPlayerStorageItem* item, int num1, int num2, int num3);

typedef const char* (*fnScriptMethodss)();
typedef const char* (*fnScriptMethodssp1)(IPlayerStorageItem* item);
typedef const char* (*fnScriptMethodssp2)(IPlayerStorageItem* item1, IPlayerStorageItem* item2);
typedef const char* (*fnScriptMethodsss1)(const char* str1);
typedef const char* (*fnScriptMethodsss2)(const char* str1, const char* str2);
typedef const char* (*fnScriptMethodsss3)(const char* str1, const char* str2, const char* str3);
typedef const char* (*fnScriptMethodssn1)(int num1);
typedef const char* (*fnScriptMethodssn2)(int num1, int num2);
typedef const char* (*fnScriptMethodssn3)(int num1, int num2, int num3);
typedef const char* (*fnScriptMethodssp1s1)(IPlayerStorageItem* item, const char* str1);
typedef const char* (*fnScriptMethodssp1s2)(IPlayerStorageItem* item, const char* str1, const char* str2);
typedef const char* (*fnScriptMethodssp1s3)(IPlayerStorageItem* item, const char* str1, const char* str2, const char* str3);
typedef const char* (*fnScriptMethodssp1n1)(IPlayerStorageItem* item, int num1);
typedef const char* (*fnScriptMethodssp1n2)(IPlayerStorageItem* item, int num1, int num2);
typedef const char* (*fnScriptMethodssp1n3)(IPlayerStorageItem* item, int num1, int num2, int num3);
#pragma endregion

class IEccoScriptSystem {
public:
	virtual void AddMethod(const char* szName, fnScriptMethodsb pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbp2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbs1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbs2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbs3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbn1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbn2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbn3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1s1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1s2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1s3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1n1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1n2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsbp1n3 pfnMethod) = 0;

	virtual void AddMethod(const char* szName, fnScriptMethodsn pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnp2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsns1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsns2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsns3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnn1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnn2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnn3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1s1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1s2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1s3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1n1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1n2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsnp1n3 pfnMethod) = 0;

	virtual void AddMethod(const char* szName, fnScriptMethodsp pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspp1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspp2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsps1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsps2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsps3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspn1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspn2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspn3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspp1s1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspp1s2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspp1s3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspp1n1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspp1n2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodspp1n3 pfnMethod) = 0;

	virtual void AddMethod(const char* szName, fnScriptMethodss pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssp1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssp2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsss1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsss2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodsss3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssn1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssn2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssn3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssp1s1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssp1s2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssp1s3 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssp1n1 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssp1n2 pfnMethod) = 0;
	virtual void AddMethod(const char* szName, fnScriptMethodssp1n3 pfnMethod) = 0;

	// if you want to include the chaiscript header
	virtual void* GetScriptEngine() const = 0;
};
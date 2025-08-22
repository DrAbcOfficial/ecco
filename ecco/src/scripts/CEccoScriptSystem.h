#pragma once
#include "api/IEccoScriptSystem.h"

class CEccoScriptSystem : public IEccoScriptSystem {
public:
	CEccoScriptSystem();

	virtual void CreateCommand(const char* name, const char* symbol, const char* description, fnFunc callback);
	virtual Result Eval(const char* content);
	virtual void ThrowError(const char* code, const char* msg);
	virtual Object* NewObject();
	virtual Object* NewIntObject(int value);
	virtual Object* NewStringObject(const char* value, size_t len);
	virtual Object* NewDoubleObject(double value);
	virtual Object* NewDictObject();
	virtual void SetDictValue(Object* dict, const char* key, Object* value);
	virtual Object* GetDictValue(Object* dict, const char* key);
	virtual Object* NewBooleanObject(bool value);
	virtual Object* NewListObject(size_t size, Object** list);
	virtual Object* GetListObject(Object* list, size_t index);
	virtual void AppendListObject(Object* list, Object* obj);
	virtual void SetListObject(Object* list, size_t index, Object* obj);
	virtual size_t GetListSize(Object* list);
	virtual void SetObjectResult(Object* object);
	virtual void SetValue(const char* name, Object* value);
	virtual void UnsetValue(const char* name);
	virtual void ResetEnviroment();
	virtual void* GetScriptEngine() const;
};